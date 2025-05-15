#!/usr/bin/bash

echo "Removing directories build and Testing..."
rm -rf build
rm -rf Testing

echo "building with TSan..."
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TSAN=ON
# cd build
cmake --build build --config Debig --parallel 4

cd build
./test_threadsafe_storage