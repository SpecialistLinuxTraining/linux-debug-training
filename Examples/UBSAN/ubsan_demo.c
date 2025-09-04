// ubsan_demo.c
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/param.h>
#include <linux/slab.h>
#include <linux/limits.h>
#include <linux/printk.h>

static int test_case = 0;
module_param(test_case, int, 0);
MODULE_PARM_DESC(test_case, "UBSan test case selector");

static int __init ubsan_demo_init(void)
{
    pr_info("init: test_case=%d\n", test_case);

    switch (test_case)
    {
    case 0:
    { /* signed overflow */
        volatile int a = INT_MAX;
        volatile int b = 1;
        int c = a + b; /* signed overflow */
        pr_info("Test 0: signed overflow result = %d\n", c);
        break;
    }
    case 1: // requires clang compiler
    {       /* null member access: typically triggers UBSAN null check instead of just a page fault */
        struct dummy
        {
            int v;
        };
        struct dummy *p = NULL;
        /* Access a member; keep p volatile to avoid compile-time folding */
        volatile struct dummy **pp = (volatile struct dummy **)&p;
        pr_info("Test 1: null member = %d\n", (*pp)->v);
        break;
    }
    case 2:
    { /* out-of-bounds index */
        volatile int idx = 5;
        int arr[2] = {1, 2};
        arr[idx] = 10; /* out-of-bounds */
        pr_info("Test 2: arr[5] = %d\n", arr[5]);
        break;
    }
    case 3:
    { /* division by zero (runtime) */
        volatile int z = 0;
        int x = 1 / z; /* div by zero */
        pr_info("Test 3: division result = %d\n", x);
        break;
    }
    case 4:
    { /* misaligned access */
        /* Allocate memory and then force an intentionally misaligned pointer */
        void *buf = kmalloc(sizeof(long) + 1, GFP_KERNEL);
        if (!buf)
        {
            pr_err("Test 4: kmalloc failed\n");
            break;
        }
        /* Make an address that is misaligned for 'long' */
        char *cp = (char *)buf;
        volatile long *lp = (volatile long *)(cp + 1);
        *lp = 42; /* misaligned write; UBSAN alignment check (if enabled) should complain */
        pr_info("Test 4: misaligned write done\n");
        kfree(buf);
        break;
    }
    case 5:                 // requires clang compiler
    {                       /* bool value out of range (UBSAN_BOOL) */
        volatile int v = 2; /* not 0/1 */
        bool b = v;         /* UB: implementation-defined promotion can trigger UBSAN_BOOL */
        pr_info("Test 5: bool value = %d\n", b);
        break;
    }
    case 6: // requires clanf compiler
    {       /* invalid enum value (UBSAN_ENUM) */
        enum color
        {
            RED = 0,
            GREEN = 1
        } c;
        volatile int x = 3; /* outside valid enumerators */
        c = (enum color)x;  /* UB */
        pr_info("Test 6: enum value = %d\n", c);
        break;
    }
    case 7:
    {                        /* shift out of bounds (CONFIG_UBSAN_SHIFT) */
        volatile int s = 31; /* or width of int - 1 */
        volatile unsigned int v = 1;
        /* Make it out-of-bounds by shifting by the type width or using a negative shift. */
        int y = (int)(v << (s + 1)); /* shift by 32 on 32-bit int */
        pr_info("Test 7: shift result = %d\n", y);
        break;
    }
    default:
        pr_info("Invalid test_case: %d\n", test_case);
        break;
    }

    return 0;
}

static void __exit ubsan_demo_exit(void)
{
    pr_info("exit\n");
}

module_init(ubsan_demo_init);
module_exit(ubsan_demo_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("UBSan Kernel Module Demo with Selectable Test Cases");
MODULE_AUTHOR("John O'Sullivan");
