
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

static int __init kmemleak_demo_init(void)
{
    void *leak = kmalloc(1024, GFP_KERNEL);
    if (!leak)
        pr_err("kmalloc failed\n");
    else
        pr_info("kmemleak demo: leaked 1KB\n");
    return 0;
}

static void __exit kmemleak_demo_exit(void)
{
    pr_info("kmemleak demo module unloaded\n");
}

module_init(kmemleak_demo_init);
module_exit(kmemleak_demo_exit);
MODULE_LICENSE("GPL");