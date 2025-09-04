#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>

static int test_case = 0;
module_param(test_case, int, 0444);
MODULE_PARM_DESC(test_case, "Select test case: 1=OOB, 2=Use-After-Free, 3=Memory Leak");

static int __init kasan_demo_init(void)
{
    char *buf;

    pr_info("KASAN demo: module loaded with test_case=%d\n", test_case);

    switch (test_case)
    {
    case 1:
        pr_info("KASAN: Buffer overflow (out-of-bounds write)\n");
        buf = kmalloc(16, GFP_KERNEL);
        if (!buf)
            return -ENOMEM;
        memset(buf, 0xAA, 32); // OOB write
        kfree(buf);
        break;

    case 2:
        pr_info("KASAN: Use-after-free\n");
        buf = kmalloc(32, GFP_KERNEL);
        if (!buf)
            return -ENOMEM;
        kfree(buf);
        buf[0] = 'X'; // Use after free
        break;

    case 3:
        pr_info("KASAN: Memory leak (no kfree)\n");
        buf = kmalloc(128, GFP_KERNEL);
        if (!buf)
            return -ENOMEM;
        // No kfree — leak
        break;

    default:
        pr_info("KASAN: No test executed. Use test_case=1|2|3\n");
    }

    return 0;
}

static void __exit kasan_demo_exit(void)
{
    pr_info("KASAN demo module unloaded\n");
}

module_init(kasan_demo_init);
module_exit(kasan_demo_exit);
MODULE_LICENSE("GPL");