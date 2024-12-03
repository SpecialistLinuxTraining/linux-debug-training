#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile ubsan_demo
cmake -DCMAKE_BUILD_TYPE=Debug .
make
