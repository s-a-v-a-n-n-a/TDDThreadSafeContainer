#ifndef STORAGE_TEST_CPP
#define STORAGE_TEST_CPP

#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <random>

#include "../src/storage.hpp"
#include "constants.hpp"

// constructor
TEST(storage_test, test_invalid_constructor) {
    EXPECT_THROW(
        (Storage<int, std::string>(10, 10)),
        std::invalid_argument
    );
}

// store
TEST(storage_test, test_invalid_store) {
    Storage<std::string, std::string> storage(2, 1);
    storage.store(CAT_CNST, CAT_TALK_CNST);
    storage.store(DOG_CNST, DOG_TALK_CNST);
    EXPECT_THROW(
        storage.store(BIRD_CNST, BIRD_TALK_CNST),
        std::out_of_range
    );
}

// store + load
TEST(storage_test, test_store_load) {
    Storage<std::string, std::string> storage(100, 10);

    storage.store(CAT_CNST, CAT_TALK_CNST);
    EXPECT_EQ(storage.load(CAT_CNST), CAT_TALK_CNST);

    storage.store(CAT_CNST, CAT_TALK_CNST_UPD);
    EXPECT_EQ(storage.load(CAT_CNST), CAT_TALK_CNST_UPD);
}

// remove + contains
TEST(storage_test, test_remove_contains) {
    Storage<std::string, std::string> storage(100, 10);
    storage.store(CAT_CNST, CAT_TALK_CNST);
    EXPECT_EQ(storage.load(CAT_CNST), CAT_TALK_CNST);

    EXPECT_TRUE(storage.contains(CAT_CNST));
    EXPECT_FALSE(storage.contains(DOG_CNST));

    EXPECT_EQ(storage.remove(CAT_CNST), CAT_TALK_CNST);
    EXPECT_FALSE(storage.contains(CAT_CNST));
}

// unexistent load
TEST(storage_test, test_unexistant_load) {
    Storage<int, int> storage(10, 5);

    EXPECT_THROW(storage.load(1), std::out_of_range);
    EXPECT_THROW(storage.remove(1), std::out_of_range);
}

// unexistent remove
TEST(storage_test, test_unexistant_remove) {
    Storage<int, int> storage(10, 5);

    EXPECT_THROW(storage.remove(1), std::out_of_range);
}

// working with cache: load
TEST(storage_test, test_correct_storage_and_cache_work_load) {
    size_t storage_capacity = 10;
    size_t cache_capacity = 3;
    Storage<int, int> storage(storage_capacity, cache_capacity);

    for (size_t i = 0; i < storage_capacity; ++i) {
        storage.store(i, i + 1);
    }

    storage.load(0);
    storage.load(1);
    storage.load(2);
    
    storage.load(0);
    storage.load(1);

    storage.store(3, 4);

    EXPECT_TRUE(storage.contains(0));
    EXPECT_TRUE(storage.contains(1));
    EXPECT_TRUE(storage.contains(3));
    EXPECT_TRUE(storage.contains(2));

    EXPECT_EQ(storage.load(2), 3);
    storage.store(4, 5);
    
    storage.load(1);
    storage.load(2);
    storage.load(3);
    storage.load(4);
    storage.load(1);
    storage.load(4);
    EXPECT_EQ(storage.load(3), 4);
}

// working with cache: remove
TEST(storage_test, test_correct_storage_and_cache_work_remove) {
    size_t storage_capacity = 10;
    size_t cache_capacity = 3;
    Storage<int, int> storage(storage_capacity, cache_capacity);

    for (size_t i = 0; i < storage_capacity; ++i) {
        storage.store(i, i + 1);
    }

    storage.load(0);
    storage.load(1);
    storage.load(2);
    
    storage.load(0);
    storage.load(1);

    storage.store(3, 4);

    EXPECT_TRUE(storage.contains(0));
    EXPECT_TRUE(storage.contains(1));
    EXPECT_TRUE(storage.contains(3));
    EXPECT_EQ(storage.remove(2), 3);
}


// thread
// store + load + remove + contains
TEST(storage_test, test_thread_access) {
    size_t storage_size = 100;
    size_t cache_size = 10;
    Storage<int, std::string> storage(storage_size, cache_size);

    const int threads_amount = 10;
    const int operations_per_threads = 1000;

    std::vector<std::thread> threads;

    for (int thread_id = 0; thread_id < threads_amount; ++thread_id) {
        auto worker = [&storage](
            const int thread_id,
            const int operations_per_threads
        ) {
            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<> distribution(
                thread_id * 10,
                (thread_id + 1) * 10 - 1
            );

            for (int i = 0; i < operations_per_threads; ++i) {
                int key = distribution(generator);
                std::string value = std::to_string(key);

                storage.store(key, value);
                std::string result = storage.load(key);
                EXPECT_EQ(result, value);
                EXPECT_TRUE(storage.contains(key));

                if (i % 10 == 0) {
                    std::string remove_result = storage.remove(key);
                    EXPECT_EQ(remove_result, value);
                    EXPECT_FALSE(storage.contains(key));
                }
            }
        };
        threads.emplace_back(worker, thread_id, operations_per_threads);
    }

    for (auto &thread: threads) {
        thread.join();
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif // STORAGE_TEST_CPP
