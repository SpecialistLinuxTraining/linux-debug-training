#!/bin/bash
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile libefence_demo
cmake -DCMAKE_BUILD_TYPE=Debug .
make
