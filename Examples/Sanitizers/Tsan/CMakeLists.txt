cmake_minimum_required(VERSION 3.10)

project(ThreadSanitizerDemo C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -fno-omit-frame-pointer -g -O1")

add_executable(tsan_demo main.c)

target_link_libraries(tsan_demo pthread)
