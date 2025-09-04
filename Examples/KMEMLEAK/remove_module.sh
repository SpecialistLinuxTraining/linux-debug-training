#!/bin/bash
sudo rmmod kmemleak_demo

# To disable kmemleak permanently for the current boot session.
# This action is irreversible. Once off is written, you cannot re-enable kmemleak without rebooting the system.
# echo off | sudo tee /sys/kernel/debug/kmemleak >/dev/null

echo "Module removed."



