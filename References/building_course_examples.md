# Building User Space Examples

## Prerequisites

Install the necessary compilation tools:

```bash
sudo apt-get install build-essential
```

To build the examples used in this course we will use a number of tools that make the compilation of code easier.

## Cmake

CMake is a cross-platform build system generator that helps to manage the build process of software projects. It provides a simple scripting language to describe the build process for various platforms and build tools.

CMake is used to generate makefiles or project files for various build systems, such as GNU Make, Visual Studio, and Xcode, among others. It is designed to be platform-independent, which means it can be used on Windows, macOS, Linux, and other Unix-like operating systems.

CMake is widely used in the software development industry because it simplifies the build process for complex projects with many dependencies. It allows developers to define the build process once and generate the appropriate build files for different platforms and build tools, reducing the amount of time and effort required to maintain the build system.

CMake uses CMakeLists.txt files to define the build process, and it supports many programming languages, including C, C++, Java, Python, and more. It also provides a variety of built-in commands and variables to help developers configure and customize the build process.

## Sample CMakeLists.txt

To build a simple hello_word application we can create a CMakeLists.txt file that specified the build process.

```cmake
cmake_minimum_required(VERSION 2.8.12)

project(hello_world)

add_executable(hello_world main.c)
```

We can then generate a Makefile which will be used to generate the binary code.


### Makefiles

A Makefile is a text file that contains a set of instructions for the make utility to build and manage a software project. The make utility uses the Makefile to determine which files need to be recompiled and relinked based on their dependencies and timestamps.

Makefiles are commonly used in Unix and Linux environments for automating the build process of software projects. They provide a simple way to define the dependencies between source code files and the rules for compiling and linking them into executable programs.

In a Makefile, developers define targets, prerequisites, and rules. A target is the output of a build process, such as an executable or object file. Prerequisites are the input files required to build the target. Rules define how to build a target from its prerequisites, such as the commands to compile and link the source code.

Makefiles can be used with a variety of programming languages, including C, C++, Java, Python, and more. They can also be used to automate tasks beyond software building, such as installing software, running tests, and cleaning up project files.

Makefiles are particularly useful for managing complex software projects with many source code files and dependencies. They provide a standardized way to build the project and ensure that the build process is efficient and reliable.

### Generating a Makefile

We can generate a Makefile by invoking the CMake command.

```sh
cmake -DCMAKE_BUILD_TYPE=Debug .
```

### Building the Code

Once the Makefile is created we can build the binary by typing: make

```sh
-- The C compiler identification is GNU 11.3.0
-- The CXX compiler identification is GNU 11.3.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/johnos/helloworld
[ 50%] Building C object CMakeFiles/hello_world.dir/main.c.o
[100%] Linking C executable hello_world
[100%] Built target hello_world
```


# Building Kernel Modules


## Prerequisites

In order to build a kernel module we must first install the kernel headers for our current version of the kernel

```bash
sudo apt-get install linux-headers-$(uname -r)
```

