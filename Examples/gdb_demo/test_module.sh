# populate 100 nodes into list/xarray/rbtree
echo 100 | sudo tee /sys/kernel/debug/gdb_demo/populate

# start/stop kthread and timer
echo start | sudo tee /sys/kernel/debug/gdb_demo/thread
echo start | sudo tee /sys/kernel/debug/gdb_demo/timer

# see basic counters
sudo cat /sys/kernel/debug/gdb_demo/stats

# trigger a breakpoint (drops into KDB/KGDB)
echo 1 | sudo tee /sys/kernel/debug/gdb_demo/break
