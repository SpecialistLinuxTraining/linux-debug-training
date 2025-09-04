#!/bin/bash
sudo dmesg -C
sudo insmod dev_log_demo.ko
echo "Module inserted. Use 'dmesg' or 'journalctl -k' to view logs."
