#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>

static int crash_module_init(void)
{
    int *p = NULL;
    printk(KERN_INFO "Attempting to dereference NULL pointer...\n");
    printk(KERN_INFO "Value at address p: %d\n", *p); // This line will cause the oops!
    return 0;                                         // This line will likely not be reached
}

static void crash_module_exit(void)
{
    printk(KERN_INFO "crash_module exited.\n");
}

module_init(crash_module_init);
module_exit(crash_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John O'Sullivan");
MODULE_DESCRIPTION("Causes Kernel Crash");
MODULE_VERSION("0.1");
