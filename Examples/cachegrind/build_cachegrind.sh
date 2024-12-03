#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile cachegrind
cmake -DCMAKE_BUILD_TYPE=Debug .
make
