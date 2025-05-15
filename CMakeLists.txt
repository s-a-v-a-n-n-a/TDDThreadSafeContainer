cmake_minimum_required(VERSION 3.15)
project(thread_safe_storage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER /usr/bin/c++)

set(CMAKE_CXX_FLAGS "-Wall -fprofile-arcs -ftest-coverage -fkeep-static-functions -fkeep-inline-functions --coverage -pthread -fsanitize=thread")
set(CMAKE_EXE_LINKER_FLAGS "-lgcov --coverage")

add_subdirectory(tests/googletest)
include_directories(
    ${CMAKE_SOURCE_DIR}/tests/googletest/include
    ${CMAKE_SOURCE_DIR}/tests/googletest/
)

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
    PUBLIC gtest gtest_main
    PRIVATE gcov
)
# target_link_libraries(test_threadsafe_storage gtest gtest_main)
