#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <list>
#include <mutex>
#include <optional>
#include <iostream>
#include <stdexcept>
#include <shared_mutex>
#include <unordered_map>


template<typename Key, typename Value>
class Storage {
public:
    Storage(size_t storage_size, size_t cache_size)
        : capacity(storage_size), cache_capacity(cache_size) {
        if (cache_size >= storage_size) {
            throw std::invalid_argument(
                "LRU cache capacity should be less than global storage capacity"
            );
        }
    }

    void store(const Key &key, const Value &value);
    Value load(const Key &key);    

    Value remove(const Key &key);
    bool contains(const Key &key);

private:
    std::unordered_map<Key, Value> storage;
    
    mutable std::shared_mutex mutex;
    mutable std::mutex cache_mutex;
    
    std::list<Key> lru_cache_keys;
    std::unordered_map<
        Key,
        typename std::list<Key>::iterator
    > cache_keys;
    std::unordered_map<Key, Value> lru_cache_values;

    const size_t capacity;
    const size_t cache_capacity;

    void lru_cache_add(const Key &key, const Value &value);
};

template<typename Key, typename Value>
void Storage<Key, Value>::store(const Key &key, const Value &value) {
    {    
        std::unique_lock cache_lock(cache_mutex);
        auto cache_iter = cache_keys.find(key);
        if (cache_iter != cache_keys.end()) {
            lru_cache_values[key] = value;
            lru_cache_keys.splice(
                lru_cache_keys.begin(),
                lru_cache_keys,
                cache_iter->second
            );
        }
    }
    
    std::unique_lock lock(mutex);
    auto storage_iter = storage.find(key);
    if (storage_iter == storage.end() && storage.size() >= capacity) {
        throw std::out_of_range("Storage is already full");
    }
    storage[key] = value;
    lock.unlock();
    lru_cache_add(key, value);
}

template<typename Key, typename Value>
Value Storage<Key, Value>::load(const Key &key) {
    {
        std::unique_lock cache_lock(cache_mutex);
        auto cache_iter = cache_keys.find(key);
        if (cache_iter != cache_keys.end()) {
            std::lock_guard promote_lock(mutex);
            lru_cache_keys.splice(
                lru_cache_keys.begin(),
                lru_cache_keys,
                cache_iter->second
            );
            return lru_cache_values[key];
        }
    }
    
    std::unique_lock lock(mutex);
    auto storage_iter = storage.find(key);
    if (storage_iter == storage.end()) {
        throw std::out_of_range("Key not found");
    }
    auto value = storage_iter->second;
    lock.unlock();
    lru_cache_add(key, value);
    return value;
}

template<typename Key, typename Value>
Value Storage<Key, Value>::remove(const Key &key) {
    if (!contains(key)) {
        throw std::out_of_range("Key not found");
    }
    
    std::unique_lock cache_lock(cache_mutex);
    auto cache_iter = cache_keys.find(key);
    if (cache_iter != cache_keys.end()) {
        auto removed = lru_cache_values[key];
        lru_cache_values.erase(*cache_iter->second);
        cache_keys.erase(cache_iter);
        lru_cache_keys.remove(key);
        cache_lock.unlock();

        std::lock_guard storage_lock(mutex);
        storage.erase(storage.find(key));
        return removed;
    } else {
        std::lock_guard lock(mutex);
        auto removed = storage[key];
        storage.erase(storage.find(key));
        return removed;
    }
}

template<typename Key, typename Value>
bool Storage<Key, Value>::contains(const Key &key) {
    {
        std::unique_lock cache_lock(cache_mutex);
        if (cache_keys.find(key) != cache_keys.end()) {
            return true;
        }
    }

    std::lock_guard lock(mutex);
    return storage.find(key) != storage.end();
}

template<typename Key, typename Value>
void Storage<Key, Value>::lru_cache_add(const Key &key, const Value &value) {
    std::lock_guard lock(cache_mutex);

    if (cache_keys.find(key) != cache_keys.end()) {
        return;
    }

    if (lru_cache_keys.size() >= cache_capacity) {
        auto last = lru_cache_keys.end();
        --last;
        lru_cache_values.erase(*last);
        cache_keys.erase(*last);
        lru_cache_keys.pop_back();
    }

    lru_cache_keys.push_front(key);
    cache_keys[key] = lru_cache_keys.begin();
    lru_cache_values[key] = value;
}

#endif // CONTAINER_HPP
