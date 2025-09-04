#!/bin/bash
sudo dmesg -C
sudo insmod log_demo.ko
echo "Module inserted. Use 'dmesg' or 'journalctl -k' to view logs."
