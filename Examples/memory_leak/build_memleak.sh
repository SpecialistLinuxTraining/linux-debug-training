#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile mem_leak
cmake -DCMAKE_BUILD_TYPE=Debug .
make
