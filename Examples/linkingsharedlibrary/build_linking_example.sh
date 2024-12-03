#!/bin/bash
# Note: This example assumes you have built and installed the helloworldsharedlibrary example
rm -rf Makefile CMakeFiles CMakeCache.txt linked_shared_library_example cmake_install.cmake
cmake -DCMAKE_BUILD_TYPE=Debug .
make