cmake_minimum_required(VERSION 3.10)

project(MemorySanitizerDemo C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=memory -fno-omit-frame-pointer -g -O1")

add_executable(msan_demo main.c)
