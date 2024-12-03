cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

# Common source files
set(SOURCE_FILES main.c)

# Debug configuration flags
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g -Wall -fdump-tree-all")
add_executable(optimization_example_debug ${SOURCE_FILES})

# Release configuration flags
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -fdump-tree-all")
add_executable(optimization_example_release ${SOURCE_FILES})

# Generate assembly code for debug
add_custom_command(TARGET optimization_example_debug POST_BUILD
    COMMAND ${CMAKE_C_COMPILER} -S -O0 -g ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILES} -o ${CMAKE_CURRENT_BINARY_DIR}/optimization_example_debug.s
    COMMENT "Generating assembly code for debug"
)

# Generate assembly code for release
add_custom_command(TARGET optimization_example_release POST_BUILD
    COMMAND ${CMAKE_C_COMPILER} -S -O3 ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILES} -o ${CMAKE_CURRENT_BINARY_DIR}/optimization_example_release.s
    COMMENT "Generating assembly code for release"
)