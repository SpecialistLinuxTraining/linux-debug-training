#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile tsan_demo
cmake -DCMAKE_BUILD_TYPE=Debug .
make
