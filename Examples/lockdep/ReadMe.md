## Overview

This module demonstrates the lockdep feature:

- Immediate feedback – Lockdep emits the warning the moment an inconsistent order appears, long before any real deadlock.
- Back-trace & dependency graph – the report prints the stack where each lock was taken, plus a minimal “who-needs-who” graph, making the bug easy to trace.
  Kernel Documentation
- No hang required – thanks to static analysis of the lock graph, the kernel keeps running.

## Enabling Lockdep in your custom kernel

```bash


make menuconfig
```

- Kernel hacking → Debug Lockups and Hangs → Lock debugging: prove locking correctness (LOCKDEP)
- Keep CONFIG_DEBUG_MUTEXES, CONFIG_DEBUG_SPINLOCK, and friends selected (they’re auto-enabled).
- Boot with lockdep messages enabled (default when built in, or add lockdep to the kernel command line).
