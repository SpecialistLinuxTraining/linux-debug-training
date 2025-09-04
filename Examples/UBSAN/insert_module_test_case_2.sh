#!/bin/bash

# Clean dmesg before running the demo
sudo dmesg -C


# Then insert our module
sudo insmod ./ubsan_demo.ko test_case=2 # out-of-bounds index test case
echo "Module inserted. Use 'dmesg' or 'journalctl -k' to view logs."

# check dmesg
sudo dmesg -w | grep -i 'UBSAN\|\[ubsan_demo\]'

