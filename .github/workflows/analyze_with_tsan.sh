#!/usr/bin/bash

cd build
TSAN_OPTIONS="halt_on_error=1" ctest --output-on-failure
