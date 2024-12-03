#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile optimization_example_* *.s
cmake -DCMAKE_BUILD_TYPE=Debug .
make