cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-O3 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-lgcov")

add_subdirectory(googletest)
include_directories(${CMAKE_SOURCE_DIR}/googletest/include ${CMAKE_SOURCE_DIR}/googletest/)

find_package(Threads REQUIRED)

add_executable(
    test_threadsafe_storage
    test_threadsafe_storage.cpp
)
set_target_properties(
    test_threadsafe_storage
    PROPERTIES
    LINKER_LANGUAGE
    CXX
)

target_link_libraries(
    test_threadsafe_storage
    PRIVATE
    storage
    Threads::Threads
    gtest gtest_main
)

target_include_directories(
    test_threadsafe_storage
    PRIVATE
    gtest gtest_main
    ${CMAKE_SOURCE_DIR}/include
)
# target_link_libraries(test_threadsafe_storage gtest gtest_main)

enable_testing()
add_test(NAME test_threadsafe_storage COMMAND test_threadsafe_storage)