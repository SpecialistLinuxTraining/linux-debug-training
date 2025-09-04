# gdb_demo

A small kernel module to demo live kernel debugging with KGDB/KDB and the lx-\* GDB Python helpers. It gives usa clean breakpoint entry, a kthread and timer to watch in lx-ps and lx-timerlist, and three common container types to poke at.

## Breakpoint trigger

- kgdb_breakpoint() callable on demand via debugfs file break.
- Optional autobreak=1 module param to halt during init.

## Elements

- kthread: gdb_demo/thread runs a loop ticking a counter every 500 ms.
- timer: gdb_demo/timer fires every 1 s while enabled.

## Memeory Structures to Inspect

- list_head list of struct demo_node. It uses the kernel’s struct list_head, which is a circular, doubly linked, intrusive list.
- xarray indexed by id.
- rbtree keyed by key. An RB-tree (red–black tree) is a self-balancing binary search tree. It keeps keys in sorted order and guarantees that common operations run in a reasonable time even in the worst case.

## DebugFS control interface at /sys/kernel/debug/gdb_demo/

- break (w) any write triggers kgdb_breakpoint()
- populate (w) echo N inserts N nodes into list/xarray/rbtree
- clear (w) removes all nodes
- thread (w) start or stop
- timer (w) start or stop
- stats (r) counters and state

```bash
# after the kernel is stopped (SysRq-g or writing to break)

(gdb) target remote /dev/pts/<kgdb-pty>
(gdb) lx-symbols # fixes KASLR, loads module symbols too (if available)

# if you prefer manual module symbols (always works):

(gdb) add-symbol-file /path/to/gdb_demo.ko \
 0x$(cat /sys/module/gdb_demo/sections/.text) \
    -s .data 0x$(cat /sys/module/gdb_demo/sections/.data) \
 -s .bss 0x$(cat /sys/module/gdb_demo/sections/.bss)

(gdb) lx-ps

(gdb) lx-timerlist

(gdb) lx-lsmod
(gdb) lx-symbols

(gdb) lx-dmesg
(gdb) lx-cmdline

(gdb) p gdb_demo_init # confirm symbols
(gdb) p demo_list
(gdb) p _(struct demo_node _)demo_list.next

(gdb) p demo_xa

(gdb) p demo_rb
```
