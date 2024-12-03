#!/bin/bash

rm -rf Makefile CMakeFiles CMakeCache.txt schedule cmake_install.cmake
cmake -DCMAKE_BUILD_TYPE=Debug .
make