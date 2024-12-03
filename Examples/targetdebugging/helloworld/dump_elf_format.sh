#!/bin/bash
readelf -h hello_world
objdump -s -j .text hello_world
objdump -s -j .rodata hello_world
objdump -s -j .data hello_world
objdump -s -j .debug_info hello_world
