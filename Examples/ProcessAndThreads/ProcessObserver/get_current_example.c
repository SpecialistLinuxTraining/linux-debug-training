#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kthread.h> // Include kthread.h
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A kernel module that periodically shows the current process");
MODULE_VERSION("1.0");

static int interval = 5000; // Interval in milliseconds (5 seconds)

module_param(interval, int, 0644); // Allow specifying the interval as a module parameter

static struct task_struct *kthread_task;

static int kthread_function(void *data)
{
    while (!kthread_should_stop())
    {

        struct task_struct *current_task;

        // Get a reference to the current process
        current_task = get_current();

        // Print information about the current process
        printk(KERN_INFO "Current process:\n");
        printk(KERN_INFO "PID: %d\n", current_task->pid);
        printk(KERN_INFO "Name: %s\n", current_task->comm);

        // Sleep for the specified interval (in milliseconds)
        msleep(interval);
    }

    return 0;
}

static int __init get_current_example_init(void)
{
    kthread_task = kthread_run(kthread_function, NULL, "get_current_example");
    if (IS_ERR(kthread_task))
    {
        printk(KERN_ERR "Failed to create kthread\n");
        return PTR_ERR(kthread_task);
    }

    return 0; // Initialization successful
}

static void __exit get_current_example_exit(void)
{
    kthread_stop(kthread_task);
    printk(KERN_INFO "Module unloaded\n");
}

module_init(get_current_example_init);
module_exit(get_current_example_exit);
