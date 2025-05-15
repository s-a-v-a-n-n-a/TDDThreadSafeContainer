#!/usr/bin/bash

echo "testing..."
cd build
./test_threadsafe_storage

echo "gathering coverage..."
lcov --directory . --capture --output-file coverage.info --rc geninfo_unexecuted_blocks=1
lcov --remove coverage.info '*/googletest/*' '/usr/*' --output-file coverage.filtered.info
lcov --list coverage.filtered.info
genhtml -o cov-report/ coverage.filtered.info
