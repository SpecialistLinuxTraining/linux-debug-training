#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile write_example read_example
cmake -DCMAKE_BUILD_TYPE=Debug .
make
