#!/bin/bash


# Insert our module
sudo insmod ./kasan_demo.ko test_case=1
echo "Module inserted. Use 'dmesg' or 'journalctl -k' to view logs."
lsmod | grep kasan
# check dmesg
sudo dmesg | grep -i kasan