#!/bin/bash


sudo mount -t debugfs nodev /sys/kernel/debug

sudo insmod kmemleak_demo.ko

echo scan | sudo tee /sys/kernel/debug/kmemleak



sudo cat /sys/kernel/debug/kmemleak






