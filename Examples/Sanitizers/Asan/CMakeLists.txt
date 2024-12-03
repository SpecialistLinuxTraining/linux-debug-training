cmake_minimum_required(VERSION 3.10)

project(AddressSanitizerDemo C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g -O1")

add_executable(asan_demo main.c)
