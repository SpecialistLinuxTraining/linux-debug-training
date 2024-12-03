cmake_minimum_required(VERSION 3.10)

project(LibefenceDemo C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -g -O1")

add_executable(libefence_demo main.c)
