#!/bin/bash
# stop moving parts
echo stop | sudo tee /sys/kernel/debug/gdb_demo/thread
echo stop | sudo tee /sys/kernel/debug/gdb_demo/timer
echo 1 | sudo tee /sys/kernel/debug/gdb_demo/clear

# remove module
sudo rmmod gdb_demo




