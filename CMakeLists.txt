cmake_minimum_required(VERSION 3.15)
project(storage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTING "Enable testing" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

add_library(storage STATIC src/storage.hpp)
set_target_properties(storage PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(storage PUBLIC include)

if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(storage PRIVATE -O3 -Wall -fprofile-arcs -ftest-coverage)
        target_link_libraries(storage PRIVATE -lgcov)
    endif()
endif()
