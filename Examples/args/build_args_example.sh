#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile args_example
cmake -DCMAKE_BUILD_TYPE=Debug .
make