#!/bin/bash

sudo mount -t debugfs none /sys/kernel/debug 2>/dev/null || true


# optional: autobreak=1 to trap immediately in init
sudo insmod ./gdb_demo.ko autobreak=0


sudo ls /sys/kernel/debug/gdb_demo

sudo lsmod | grep gdb_demo
