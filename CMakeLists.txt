cmake_minimum_required(VERSION 3.15)
project(thread_safe_storage)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER /usr/bin/c++)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -pthread")

if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fkeep-static-functions -fkeep-inline-functions --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "-lgcov --coverage")
endif()

if(ENABLE_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif()

file(GLOB_RECURSE SOURCE_EXE "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*.hpp")
file(
    GLOB_RECURSE
    TEST_EXE
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
    "${CMAKE_SOURCE_DIR}/tests/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
)

add_executable(test_threadsafe_storage ${TEST_EXE} ${LIB})

target_link_libraries(test_threadsafe_storage
    PRIVATE
    GTest::GTest
    GTest::Main
    gcov
)
