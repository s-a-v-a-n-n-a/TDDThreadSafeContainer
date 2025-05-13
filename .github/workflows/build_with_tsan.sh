#!/usr/bin/bash

echo "Removing directories build and Testing..."
rm -rf build
rm -rf Testing

echo "building..."
cmake -B build  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=thread" -DENABLE_TESTING=ON
cmake --build build --config Debug --parallel 4
