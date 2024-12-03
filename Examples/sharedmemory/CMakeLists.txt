cmake_minimum_required(VERSION 3.0)
project(shared_memory_example)

set(CMAKE_C_STANDARD 11)

add_executable(write_example write_example.c)
add_executable(read_example read_example.c)

# librt is a POSIX Real-Time Extensions library that provides various real-time functions.
# On some systems or toolchains, certain real-time functions may already be included in the standard C library (libc),
# so explicitly linking with librt may not be necessary.
# On glibc-based Linux systems, it's common to use -lrt to ensure proper linkage when using functions like shm_open.
target_link_libraries(write_example rt)
target_link_libraries(read_example rt)
