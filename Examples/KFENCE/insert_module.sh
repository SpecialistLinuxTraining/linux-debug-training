#!/bin/bash
# Clear dnesg
echo "clearing dmesg"
sudo dmesg -C

# Then insert our module
echo "inserting dmesg"
sudo insmod ./kfence_demo.ko test_case=2


# check dmesg
sudo dmesg  | grep -i '\[KFENCE\]'
