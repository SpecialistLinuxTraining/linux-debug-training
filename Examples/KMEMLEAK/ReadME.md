make linux-menuconfig

-> Kernel hacking │  
 │ -> Memory Debugging │  
 │ (1) -> Kernel memory leak detector (DEBUG_KMEMLEAK [=y])

---

echo scan > /sys/kernel/debug/kmemleak
[ 62.888171] kmemleak: 1 new suspected memory leak (see /sys/kernel/debug/kmemleak)

cat /sys/kernel/debug/kmemleak

unreferenced object 0x83f92800 (size 1024):
comm "insmod", pid 155, jiffies 4294939410
hex dump (first 32 bytes):
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
backtrace (crc 9c453d41):
kmemleak_alloc+0x44/0x4c
\_\_kmalloc_cache_noprof+0x2d4/0x310
0x7f08b024
do_one_initcall+0x50/0x214
do_init_module+0x5c/0x228
init_module_from_file+0x9c/0xd8
sys_finit_module+0x1ac/0x304
ret_fast_syscall+0x0/0x54

Explanation:

unreferenced object 0x83f92800 (size 1024) Kmemleak found a memory object at address 0x83f92800 of size 1024 bytes (1 KB) that is not reachable by any known pointer - a likely memory leak.

hex dump ... Shows the first 32 bytes of the leaked memory region, all zeroed — typical for an allocated but unused block.
