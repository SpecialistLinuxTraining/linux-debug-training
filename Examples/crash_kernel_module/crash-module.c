#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>

static int crash_module_init(void)
{
	int *p = 1;
	printk("%d\n", *p);
	return 0;
}

static void crash_module_exit(void)
{
	return;
}

module_init(crash_module_init);
module_exit(crash_module_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Causes Kernel Crash");
