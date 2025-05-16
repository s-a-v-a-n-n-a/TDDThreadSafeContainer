#!/usr/bin/bash

echo "testing..."
cd build
./test_threadsafe_storage

LCOV_ARGS=(--rc lcov_branch_coverage=1)

echo "gathering coverage..."
lcov --directory .. --capture --output-file coverage.info "${LCOV_ARGS[@]}"
lcov --remove coverage.info '*/tests/*' '/usr/*' --output-file coverage.filtered.info "${LCOV_ARGS[@]}"
lcov --summary coverage.filtered.info "${LCOV_ARGS[@]}"
genhtml -o cov-report/ coverage.filtered.info "${LCOV_ARGS[@]}"
