# pr_format demo

```
%p: Displays a hashed pointer.
%pe: Converts an error number to a human-readable string.
%pS: Displays a function symbol.
%pa: Displays a physical address.
%pK: Displays a kernel pointer (hashed unless kptr_restrict=0).
%pOF: Displays a device-tree node (if available).
```

**Note** on hashed pointers:
Hasing a pointer does not modify the actual pointer—only its printed representation.The hashing behavior is controlled by kptr_restrict (sysctl setting).

```
echo 0 | sudo tee /proc/sys/kernel/kptr_restrict # Show full pointers
echo 1 | sudo tee /proc/sys/kernel/kptr_restrict # Default: Hash for normal users
echo 2 | sudo tee /proc/sys/kernel/kptr_restrict # Always hash pointers

```
