#!/usr/bin/bash

echo "Removing directories build and Testing..."
rm -rf build
rm -rf Testing

echo "building..."
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
# cd build
cmake --build build --config Debig --parallel 4
