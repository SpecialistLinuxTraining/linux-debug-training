cmake_minimum_required(VERSION 3.5)

project(my_library)

set(CMAKE_CXX_STANDARD 11)

# Define the source files for your library
set(SOURCE_FILES
    src/my_shared_library.c
)

# Define the header files for your library
set(HEADER_FILES
    include/my_library/my_shared_library.h
)

# Create the shared library
add_library(my_library SHARED ${SOURCE_FILES} ${HEADER_FILES})

# Add include directories to the library target
target_include_directories(my_library PUBLIC include)

# Set library version
set_target_properties(my_library PROPERTIES VERSION 1.0.0)

# Set library installation directory
install(TARGETS my_library DESTINATION lib)

# Install header files
install(DIRECTORY include/my_library DESTINATION include)