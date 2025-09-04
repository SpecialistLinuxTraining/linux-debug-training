#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John OSullivan");
MODULE_DESCRIPTION("Kernel Logging Demonstration Module");
MODULE_VERSION("1.0");

static int __init log_demo_init(void)
{
    printk(KERN_INFO "log_demo: Module loaded!\n");

    // Demonstrating printk() logging levels
    printk(KERN_EMERG "[%s] Emergency! System is crashing!\n", KBUILD_MODNAME);
    printk(KERN_ALERT "[%s] Alert! Immediate action needed!\n", KBUILD_MODNAME);
    printk(KERN_CRIT "[%s] Critical condition detected!\n", KBUILD_MODNAME);
    printk(KERN_ERR "[%s] Error encountered!\n", KBUILD_MODNAME);
    printk(KERN_WARNING "[%s] Warning: Possible issue detected.\n", KBUILD_MODNAME);
    printk(KERN_NOTICE "[%s] Notice: Normal but important event.\n", KBUILD_MODNAME);
    printk(KERN_INFO "[%s] Informational message.\n", KBUILD_MODNAME);
    printk(KERN_DEBUG "[%s] Debugging information.\n", KBUILD_MODNAME);

    // Demonstrating pr_*() functions
    pr_emerg("[%s] Emergency! System is crashing!", KBUILD_MODNAME);
    pr_alert("[%s] Alert! Immediate action needed!", KBUILD_MODNAME);
    pr_crit("[%s] Critical condition detected!", KBUILD_MODNAME);
    pr_err("[%s] Error encountered!", KBUILD_MODNAME);
    pr_warn("[%s] Warning: Possible issue detected.", KBUILD_MODNAME);
    pr_notice("[%s] Notice: Normal but important event.", KBUILD_MODNAME);
    pr_info("[%s] Informational message.", KBUILD_MODNAME);
    pr_debug("[%s] Debugging information.", KBUILD_MODNAME);

    return 0;
}

static void __exit log_demo_exit(void)
{
    printk(KERN_INFO "log_demo: Module unloaded!\n");
}

module_init(log_demo_init);
module_exit(log_demo_exit);
