#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile thread
cmake -DCMAKE_BUILD_TYPE=Debug .
make
