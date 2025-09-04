// dynamic_debug_example.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/version.h> // Include for LINUX_VERSION_CODE and KERNEL_VERSION

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John O'Sullivan");
MODULE_DESCRIPTION("A kernel module to demonstrate dynamic debugging");

static int my_variable = 10;
static struct proc_dir_entry *proc_entry;
static DEFINE_MUTEX(trigger_lock);

// Function Prototypes (Forward Declarations)
static int dynamic_debug_function_one(int arg);
static void dynamic_debug_function_two(const char *message);
static int dynamic_debug_function_three(void);
static ssize_t trigger_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
static const struct proc_ops trigger_fops = {
    .proc_write = trigger_write,
};
#else
static const struct file_operations trigger_fops = {
    .write = trigger_write,
};
#endif

static int __init dynamic_debug_example_init(void)
{
    pr_debug("dynamic_debug_example: Module loaded successfully.\n");
    pr_info("This is an info message.\n");
    printk(KERN_WARNING "This is a warning message.\n");

    if (my_variable > 5)
    {
        pr_debug("dynamic_debug_example: my_variable (%d) is greater than 5 (in init).\n", my_variable);
    }
    int result_dynamic_debug_function_one = dynamic_debug_function_one(7);
    dynamic_debug_function_two("Hello from init!");
    int result_some_other = dynamic_debug_function_three();

    pr_debug("dynamic_debug_example: dynamic_debug_function_one returned: %d\n", result_dynamic_debug_function_one);
    pr_debug("dynamic_debug_example: dynamic_debug_function_three returned: %d\n", result_some_other);

    proc_entry = proc_create("trigger_dynamic_debug", 0666, NULL, &trigger_fops);
    if (!proc_entry)
    {
        pr_err("dynamic_debug_example: Failed to create proc entry.\n");
        return -ENOMEM;
    }

    return 0;
}

static void __exit dynamic_debug_example_exit(void)
{
    remove_proc_entry("trigger_dynamic_debug", NULL);
    pr_debug("dynamic_debug_example: Module unloaded successfully.\n");
}

static int dynamic_debug_function_one(int arg)
{
    pr_debug("dynamic_debug_example: Inside dynamic_debug_function_one with arg = %d.\n", arg);
    if (arg % 2 == 0)
    {
        pr_debug("dynamic_debug_example: Argument %d is even.\n", arg);
        return 0;
    }
    else
    {
        pr_debug("dynamic_debug_example: Argument %d is odd.\n", arg);
        return -1;
    }
}

static void dynamic_debug_function_two(const char *message)
{
    pr_debug("dynamic_debug_example: In dynamic_debug_function_two, called with message: %s\n", message);
}

static int dynamic_debug_function_three(void)
{
    int result = 42;
    pr_debug("dynamic_debug_example: In dynamic_debug_function_three, result is %d.\n", result);
    return result;
}

static ssize_t trigger_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
    char kernel_buffer[128];
    int ret;

    if (!mutex_trylock(&trigger_lock))
    {
        return -EBUSY;
    }

    if (count > sizeof(kernel_buffer) - 1)
    {
        count = sizeof(kernel_buffer) - 1;
    }

    ret = copy_from_user(kernel_buffer, ubuf, count);
    if (ret < 0)
    {
        mutex_unlock(&trigger_lock);
        return ret;
    }

    kernel_buffer[count] = '\0';
    dynamic_debug_function_two(kernel_buffer);

    mutex_unlock(&trigger_lock);
    return count;
}

module_init(dynamic_debug_example_init);
module_exit(dynamic_debug_example_exit);