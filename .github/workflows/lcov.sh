#!/usr/bin/bash

echo "testing..."
cd build
./tests/test_threadsafe_storage

echo "gathering coverage..."
lcov --capture --directory src --output-file coverage.info
genhtml -o cov-report/ coverage.info
