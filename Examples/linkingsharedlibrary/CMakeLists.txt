cmake_minimum_required(VERSION 3.5)

project(my_project)

# Set the C++ standard to use
set(CMAKE_CXX_STANDARD 11)

# Locate the shared library
find_library(MY_LIBRARY my_library /usr/local/lib)

# Add the include directory for the library
include_directories(/usr/local/include)

# Create an executable target for the main file
add_executable(linked_shared_library_example main.c)

# Link the executable with the shared library
target_link_libraries(linked_shared_library_example ${MY_LIBRARY})