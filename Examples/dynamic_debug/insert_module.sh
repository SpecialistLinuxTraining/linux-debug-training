#!/bin/bash
sudo dmesg -C
sudo insmod dynamic_debug_example.ko
echo "Module inserted."
