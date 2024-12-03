#!/bin/bash
rm -rf Makefile CMakeFiles CMakeCache.txt *.so *.so.* cmake_install.cmake
cmake -DCMAKE_BUILD_TYPE=Debug .
make
