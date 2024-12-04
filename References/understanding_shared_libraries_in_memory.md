# A Detailed look at how Shared Libraries link to a main Program

## Introduction

In C programming, shared libraries are a means of allowing multiple programs to share the same code; this leads to more efficient use of memory and easier management of updates and dependencies. Unlike static libraries, which are embedded directly into the executable at compile time, shared libraries are dynamically loaded into memory at runtime. This dynamic linking allows programs to run more efficiently and saves system resources.

In this article, we will explore how shared libraries link to a main program and the various tools and techniques that help us understand their inner workings. Specifically, we will delve into the following topics:

- Building Shared Libraries: We'll start by learning how to create shared libraries in C and link them to a main program.
- Analyzing the Binary with _readelf_: Next, we'll explore how to use _readelf_ to examine the details of the binary file and identify how shared libraries are integrated into the program.
- Using _pmap_ to Inspect Memory: We'll see how the _pmap_ tool can be used to map out the memory layout of a program and understand how shared libraries are loaded at runtime.
- Multiple Instances of a Shared Library: It's common to observe multiple instances of the same shared library loaded into memory. We'll discuss why this happens and how the operating system manages it.
- Using _objdump_ to View Virtual Addresses: We'll learn how to use _objdump_ to inspect the program's virtual addresses and understand how the system manages memory space.
- Using _GDB_ to Find Addresses: Finally, we will use _GDB_, the GNU Debugger, to find and inspect the addresses of functions and variables within shared libraries, giving us insight into how the dynamic linking process occurs at runtime.


## Example Code

To understand how shared libraries are structured and managed in memory we will use the following simple program throughout this article.

```c
#include <stdio.h>
#include <unistd.h>
#include <my_library/my_shared_library.h>

void main()
{
    print_hello_world(); // this is a call to the hello world function in the shared library
    while (1)            // run indefinitely so that we can examine memory
    {
        sleep(10);
    }
}


```

## Building the Code

We can build the code with the following CMake File.

```cmake
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

```

To build the code we can run:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug .
make
```

## Analysing the Binary with readelf

We can use readelf to view the shared libraries our binary depends on:

```bash
readelf -d linked_shared_library_example | grep NEEDED
 0x0000000000000001 (NEEDED)             Shared library: [libmy_library.so.1.0.0]
 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]
```
We can also use readelf to find the location or runpath for the shared library

```bash
readelf -d linked_shared_library_example | grep -i runpath
 0x000000000000001d (RUNPATH)            Library runpath: [/usr/local/lib]
```

We can use ldd to list the shared libraries and their locations in virtual memory

```bash
ldd linked_shared_library_example 
        linux-vdso.so.1 (0x00007ffff7fc1000)
        libmy_library.so.1.0.0 => /usr/local/lib/libmy_library.so.1.0.0 (0x00007ffff7fb1000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ffff7c00000)
        /lib64/ld-linux-x86-64.so.2 (0x00007ffff7fc3000)
```

## Using pmap

We can also use pmap to view the physical addresses of any copies of the linked shared library in memory

```bash
pmap 997260 | grep libmy_library
00007ffff7fb6000      4K r---- libmy_library.so.1.0.0
00007ffff7fb7000      4K r-x-- libmy_library.so.1.0.0
00007ffff7fb8000      4K r---- libmy_library.so.1.0.0
00007ffff7fb9000      4K r---- libmy_library.so.1.0.0
00007ffff7fba000      4K rw--- libmy_library.so.1.0.0

```

## Why we see Multiple Instances of a Shared Library

You may see multiple instances of a shared library in the output of pmap, it means that the program has loaded multiple copies of that library into memory. There can be several reasons why this happens:

- Different versions: The program may be designed to work with multiple versions of the same shared library, and may load each version separately. In this case, each instance of the library will have a different version number or filename.
- Copy-on-write: When a program forks, the child process shares the same memory with the parent process until one of them writes to a memory page. At that point, the page is copied, and the two processes have separate copies of the page. This is called copy-on-write (COW) behavior. If a shared library is loaded by the parent process and then the child process forks, both processes will share the same copy of the library until one of them modifies it. At that point, the modified pages will be copied, and the two processes will have separate copies of those pages.
- Position-independent code: Some shared libraries contain position-independent code (PIC), which means that the code can be loaded at different addresses in memory and still work correctly. If a program loads such a library at multiple addresses, each instance of the library will have a different address range in pmap.

It's important to note that having multiple instances of a shared library in memory doesn't necessarily mean that the program is using more memory than necessary. If the library is loaded as read-only and contains shared code or data, the operating system can share the physical memory pages between multiple instances of the library, reducing memory usage.

## Using Objdump to View Virtual Addresses

We can view the virtual address of the functions in a shared library using objdump. The first column will show the virtual memory offset of the function with the library.

```bash
objdump -T /usr/local/lib/libmy_library.so

/usr/local/lib/libmy_library.so:     file format elf64-x86-64

DYNAMIC SYMBOL TABLE:
0000000000000000  w   D  *UND*	0000000000000000  Base        _ITM_deregisterTMCloneTable
0000000000000000  w   D  *UND*	0000000000000000  Base        __gmon_start__
0000000000000000      DF *UND*	0000000000000000 (GLIBC_2.2.5) fwrite
0000000000000000  w   D  *UND*	0000000000000000  Base        _ITM_registerTMCloneTable
0000000000000000  w   DF *UND*	0000000000000000 (GLIBC_2.2.5) __cxa_finalize
0000000000000000      DO *UND*	0000000000000000 (GLIBC_2.2.5) stderr
000000000000114a g    DF .text	0000000000000031  Base        second_shared_library_function
0000000000001119 g    DF .text	0000000000000031  Base        print_hello_world

```

We can also use objdump to view the virtual addresses of both the main program and the shared library.

```bash
$ objdump -T linked_shared_library_example /usr/local/lib/libmy_library.so 

linked_shared_library_example:     file format elf64-x86-64

DYNAMIC SYMBOL TABLE:
0000000000000000      DF *UND*  0000000000000000 (GLIBC_2.34) __libc_start_main
0000000000000000  w   D  *UND*  0000000000000000  Base        _ITM_deregisterTMCloneTable
0000000000000000  w   D  *UND*  0000000000000000  Base        __gmon_start__
0000000000000000      DF *UND*  0000000000000000  Base        print_hello_world
0000000000000000  w   D  *UND*  0000000000000000  Base        _ITM_registerTMCloneTable
0000000000000000      DF *UND*  0000000000000000 (GLIBC_2.2.5) sleep
0000000000000000  w   DF *UND*  0000000000000000 (GLIBC_2.2.5) __cxa_finalize


/usr/local/lib/libmy_library.so:     file format elf64-x86-64

DYNAMIC SYMBOL TABLE:
0000000000000000  w   D  *UND*  0000000000000000  Base        _ITM_deregisterTMCloneTable
0000000000000000  w   D  *UND*  0000000000000000  Base        __gmon_start__
0000000000000000      DF *UND*  0000000000000000 (GLIBC_2.2.5) fwrite
0000000000000000  w   D  *UND*  0000000000000000  Base        _ITM_registerTMCloneTable
0000000000000000  w   DF *UND*  0000000000000000 (GLIBC_2.2.5) __cxa_finalize
0000000000000000      DO *UND*  0000000000000000 (GLIBC_2.2.5) stderr
000000000000114a g    DF .text  0000000000000031  Base        second_shared_library_function
0000000000001119 g    DF .text  0000000000000031  Base        print_hello_world
```

Notice that in the main program the virtual address associated with the shared library function is 0000000000000000.

```bash
0000000000000000      DF *UND*  0000000000000000  Base        print_hello_world
```

This typically means that the symbols have not been fully resolved at the time of linking.

In other words, the program is using dynamic linking to resolve symbols at runtime, and the final virtual addresses of the symbols have not been determined yet. When you link a program with shared libraries, the linker leaves placeholders for the symbols in the program's symbol table, with virtual addresses set to zero. At runtime, the dynamic linker resolves the symbols and updates the virtual addresses in the program's symbol table.

If you want to see the actual virtual addresses of symbols in the program at runtime, you can use a debugger such as gdb. You can run the program under the debugger and use the info symbol command to show the virtual address of a symbol.

For example, if we set a breakpoint and stop our program we can then use the Debug Console to find the virtual addresses of the shared library objects.

## Using GDB to Find Addresses

```gdb
-exec info sharedlibrary
From                To                  Syms Read   Shared Object Library
0x00007ffff7fc5090  0x00007ffff7fee335  Yes         /lib64/ld-linux-x86-64.so.2
0x00007ffff7fb7060  0x00007ffff7fb717b  Yes         /usr/local/lib/libmy_library.so.1.0.0
0x00007ffff7c28700  0x00007ffff7dbaabd  Yes         /lib/x86_64-linux-gnu/libc.so.6
```

We can also request the address of a specific function.

```gdb
-exec info address print_hello_world
Symbol "print_hello_world" is a function at address 0x7ffff7fb7119.
```

**Note**: For more details on how to configure vscode to debug the file and view the gdb information see [Using VSCode to debug a C Program](using_vscode.md)
