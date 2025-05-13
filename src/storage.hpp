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
        //
    }

    void store(const Key &key, const Value &value) {}

    Value load(const Key &key) {}

    Value remove(const Key &key) {}

    bool contains(const Key &key) {
        return false;
    }

private:
    const size_t capacity;
    const size_t cache_capacity;

    void lru_cache_add(const Key &key, const Value &value) {}
};

#endif // CONTAINER_HPP
