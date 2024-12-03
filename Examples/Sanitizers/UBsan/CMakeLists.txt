cmake_minimum_required(VERSION 3.10)

project(UndefinedBehaviorSanitizerDemo C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fno-omit-frame-pointer -g -O1")

add_executable(ubsan_demo main.c)

