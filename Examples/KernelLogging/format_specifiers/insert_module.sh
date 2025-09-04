#!/bin/bash
sudo dmesg -C
sudo insmod pr_format_demo.ko
echo "Module inserted. Use 'dmesg' or 'journalctl -k' to view logs."
