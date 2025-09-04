#!/bin/bash





# Tinsert our module
sudo insmod ./kcsan_race_demo.ko
echo "Module inserted. Use 'dmesg' or 'journalctl -k' to view logs."

# We use the debug fs to observe the program
sudo sh -c 'while :; do cat /sys/kernel/debug/kcsan_race_demo/stats; sleep 0.2; done'

# check dmesg
sudo dmesg | grep -i kcsan
