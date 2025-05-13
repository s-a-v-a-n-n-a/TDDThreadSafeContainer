#!/usr/bin/bash

echo "testing..."
cd build
./tests/test_threadsafe_storage

echo "gathering coverage..."
lcov --capture --directory . --output-file coverage.info
lcov --remove coverage.info '*/tests/googletest/*' '/usr/*' --output-file coverage.filtered.info
lcov --list coverage.filtered.info
genhtml -o cov-report/ coverage.filtered.info
