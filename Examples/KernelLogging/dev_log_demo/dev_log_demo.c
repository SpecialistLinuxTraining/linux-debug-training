#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#define DEVICE_NAME "dev_log_demo"

// Define a dummy platform device
static struct platform_device *demo_device;

/* Module initialization */
static int __init dev_log_demo_init(void)
{
    struct device *dev;

    // Allocate and register the platform device
    demo_device = platform_device_alloc(DEVICE_NAME, -1);
    if (!demo_device)
        return -ENOMEM;

    if (platform_device_add(demo_device))
    {
        platform_device_put(demo_device);
        return -ENODEV;
    }

    dev = &demo_device->dev; // Get the device structure

    // Demonstrate logging at different severity levels
    dev_emerg(dev, "[%s] Emergency! System is crashing!\n", KBUILD_MODNAME);
    dev_alert(dev, "[%s] Alert! Immediate action needed!\n", KBUILD_MODNAME);
    dev_crit(dev, "[%s] Critical condition detected!\n", KBUILD_MODNAME);
    dev_err(dev, "[%s] Error encountered!\n", KBUILD_MODNAME);
    dev_warn(dev, "[%s] Warning: Possible issue detected.\n", KBUILD_MODNAME);
    dev_notice(dev, "[%s] Notice: Normal but important event.\n", KBUILD_MODNAME);
    dev_info(dev, "[%s] Informational message.\n", KBUILD_MODNAME);
    dev_dbg(dev, "[%s] Debugging information.\n", KBUILD_MODNAME);

    return 0;
}

/* Module cleanup */
static void __exit dev_log_demo_exit(void)
{
    if (demo_device)
        platform_device_unregister(demo_device);

    pr_info("[%s] Module is unloading.\n", KBUILD_MODNAME);
}

// Register module entry/exit points
module_init(dev_log_demo_init);
module_exit(dev_log_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John OSullivan");
MODULE_DESCRIPTION("Kernel logging demo using dev_*() functions");
