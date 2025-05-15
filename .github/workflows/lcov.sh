#!/usr/bin/bash

echo "testing..."
cd build
./test_threadsafe_storage

echo "gathering coverage..."
lcov --directory .. --capture --output-file coverage.info --rc lcov_function_coverage=1 --branch-coverage --rc geninfo_unexecuted_blocks=1 --ignore-errors mismatch,mismatch
lcov --remove coverage.info '*/tests/*' '/usr/*' --output-file coverage.filtered.info
lcov --summary coverage.filtered.info
genhtml -o cov-report/ coverage.filtered.info --rc lcov_branch_coverage=1 --rc geninfo_no_exception_branch=1
