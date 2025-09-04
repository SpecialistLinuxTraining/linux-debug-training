// gdb_demo.c — small kernel-debug demo module
// Features: debugfs controls, list/xarray/rbtree, kthread, timer, kgdb_breakpoint()

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/rbtree.h>
#include <linux/xarray.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/kgdb.h> /* kgdb_breakpoint() */
#include <linux/fs.h>   /* noop_llseek */
#include <linux/version.h>

#define DRV "gdb_demo"

struct demo_node
{
    struct list_head link; /* in demo_list */
    struct rb_node rb;     /* in demo_rb */
    u32 key;               /* rbtree key */
    u64 id;                /* xarray index */
    char data[32];
};

static struct dentry *dbgdir;
static struct list_head demo_list;
static struct rb_root demo_rb = RB_ROOT;
static DEFINE_SPINLOCK(demo_lock);
static DEFINE_XARRAY(demo_xa);

static struct task_struct *demo_thread;
static struct timer_list demo_timer;
static atomic_long_t ticks;
static atomic_t list_count, xa_count, rb_count;

static bool timer_running;

/* ---------- rbtree helpers ---------- */

static void demo_rb_insert(struct demo_node *node)
{
    struct rb_node **new = &demo_rb.rb_node, *parent = NULL;

    while (*new)
    {
        struct demo_node *cur = rb_entry(*new, struct demo_node, rb);
        parent = *new;
        if (node->key < cur->key)
            new = &(*new)->rb_left;
        else if (node->key > cur->key)
            new = &(*new)->rb_right;
        else
            break; /* allow duplicate: place as right child */
    }
    rb_link_node(&node->rb, parent, new);
    rb_insert_color(&node->rb, &demo_rb);
    atomic_inc(&rb_count);
}

static void demo_rb_remove(struct demo_node *node)
{
    rb_erase(&node->rb, &demo_rb);
    atomic_dec(&rb_count);
}

/* ---------- kthread & timer ---------- */

static int demo_thread_fn(void *arg)
{
    while (!kthread_should_stop())
    {
        atomic_long_inc(&ticks);
        set_current_state(TASK_INTERRUPTIBLE);
        schedule_timeout(HZ / 2); /* ~500ms */
    }
    __set_current_state(TASK_RUNNING);
    return 0;
}

static void demo_timer_fn(struct timer_list *t)
{
    atomic_long_inc(&ticks);
    if (timer_running)
        mod_timer(&demo_timer, jiffies + HZ); /* every 1s */
}

/* ---------- data structure mgmt ---------- */

static void free_all_nodes(void)
{
    struct demo_node *n, *tmp;

    spin_lock_bh(&demo_lock);

    list_for_each_entry_safe(n, tmp, &demo_list, link)
    {
        list_del(&n->link);
        demo_rb_remove(n);
        xa_erase(&demo_xa, n->id);
        kfree(n);
    }

    INIT_LIST_HEAD(&demo_list);
    xa_destroy(&demo_xa);
    xa_init(&demo_xa);

    atomic_set(&list_count, 0);
    atomic_set(&xa_count, 0);
    /* rb_count is handled in demo_rb_remove() loop */

    spin_unlock_bh(&demo_lock);
}

static int populate_nodes(unsigned int nr)
{
    unsigned int i;
    int ret = 0;

    spin_lock_bh(&demo_lock);

    for (i = 0; i < nr; i++)
    {
        struct demo_node *n = kzalloc(sizeof(*n), GFP_ATOMIC);
        if (!n)
        {
            ret = -ENOMEM;
            break;
        }

        n->key = (u32)(i * 17u + 3u); /* deterministic but non-trivial */
        n->id = i;
        snprintf(n->data, sizeof(n->data), "node%u", i);

        list_add_tail(&n->link, &demo_list);
        atomic_inc(&list_count);

        demo_rb_insert(n);

        if (xa_store(&demo_xa, n->id, n, GFP_ATOMIC))
            pr_warn(DRV ": xa_store failed for id=%llu\n", n->id);
        else
            atomic_inc(&xa_count);
    }

    spin_unlock_bh(&demo_lock);
    return ret;
}

/* ---------- debugfs: write handlers ---------- */

static ssize_t brk_write(struct file *f, const char __user *buf,
                         size_t len, loff_t *ppos)
{
    /* ignore contents; any write triggers a trap */
    kgdb_breakpoint();
    return len;
}

static ssize_t populate_write(struct file *f, const char __user *buf,
                              size_t len, loff_t *ppos)
{
    char tmp[32];
    unsigned long nr = 0;

    if (len >= sizeof(tmp))
        return -EINVAL;
    if (copy_from_user(tmp, buf, len))
        return -EFAULT;
    tmp[len] = '\0';

    if (kstrtoul(tmp, 0, &nr))
        return -EINVAL;

    if (populate_nodes(nr))
        return -ENOMEM;

    return len;
}

static ssize_t clear_write(struct file *f, const char __user *buf,
                           size_t len, loff_t *ppos)
{
    free_all_nodes();
    return len;
}

static ssize_t thread_write(struct file *f, const char __user *buf,
                            size_t len, loff_t *ppos)
{
    char tmp[16];

    if (len >= sizeof(tmp))
        return -EINVAL;
    if (copy_from_user(tmp, buf, len))
        return -EFAULT;
    tmp[len] = '\0';

    if (sysfs_streq(tmp, "start"))
    {
        if (!demo_thread)
        {
            demo_thread = kthread_run(demo_thread_fn, NULL, DRV "/thread");
            if (IS_ERR(demo_thread))
            {
                pr_err(DRV ": kthread_run failed: %ld\n", PTR_ERR(demo_thread));
                demo_thread = NULL;
                return -ECHILD;
            }
        }
    }
    else if (sysfs_streq(tmp, "stop"))
    {
        if (demo_thread)
        {
            kthread_stop(demo_thread);
            demo_thread = NULL;
        }
    }
    return len;
}

static ssize_t timer_write(struct file *f, const char __user *buf,
                           size_t len, loff_t *ppos)
{
    char tmp[16];

    if (len >= sizeof(tmp))
        return -EINVAL;
    if (copy_from_user(tmp, buf, len))
        return -EFAULT;
    tmp[len] = '\0';

    if (sysfs_streq(tmp, "start"))
    {
        if (!timer_running)
        {
            timer_running = true;
            mod_timer(&demo_timer, jiffies + HZ);
        }
    }
    else if (sysfs_streq(tmp, "stop"))
    {
        if (timer_running)
        {
            timer_running = false;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 9, 0)
            timer_shutdown_sync(&demo_timer);
#else
            del_timer_sync(&demo_timer);
#endif
        }
    }
    return len;
}

/* ---------- debugfs: read handler ---------- */

static int stats_show(struct seq_file *m, void *v)
{
    seq_printf(m, "ticks=%ld\n", atomic_long_read(&ticks));
    seq_printf(m, "list_count=%d\n", atomic_read(&list_count));
    seq_printf(m, "xa_count=%d\n", atomic_read(&xa_count));
    seq_printf(m, "rb_count=%d\n", atomic_read(&rb_count));
    seq_puts(m, "thread=");
    seq_puts(m, demo_thread ? "running\n" : "stopped\n");
    seq_puts(m, "timer=");
    seq_puts(m, timer_running ? "running\n" : "stopped\n");
    return 0;
}

static int stats_open(struct inode *inode, struct file *file)
{
    return single_open(file, stats_show, inode->i_private);
}

/* ---------- debugfs fops ---------- */

static const struct file_operations brk_fops = {
    .write = brk_write,
    .llseek = noop_llseek,
};

static const struct file_operations populate_fops = {
    .write = populate_write,
    .llseek = noop_llseek,
};

static const struct file_operations clear_fops = {
    .write = clear_write,
    .llseek = noop_llseek,
};

static const struct file_operations thread_fops = {
    .write = thread_write,
    .llseek = noop_llseek,
};

static const struct file_operations timer_fops = {
    .write = timer_write,
    .llseek = noop_llseek,
};

static const struct file_operations stats_fops = {
    .open = stats_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* ---------- module init/exit ---------- */

static bool autobreak;
module_param(autobreak, bool, 0444);
MODULE_PARM_DESC(autobreak, "Call kgdb_breakpoint() during init");

static int __init gdb_demo_init(void)
{
    int err = 0;

    INIT_LIST_HEAD(&demo_list);
    spin_lock_init(&demo_lock);
    xa_init(&demo_xa);
    atomic_long_set(&ticks, 0);
    atomic_set(&list_count, 0);
    atomic_set(&xa_count, 0);
    atomic_set(&rb_count, 0);
    timer_running = false;
    timer_setup(&demo_timer, demo_timer_fn, 0);

    dbgdir = debugfs_create_dir(DRV, NULL);
    if (!dbgdir)
    {
        pr_warn(DRV ": debugfs not available\n");
    }
    else
    {
        if (!debugfs_create_file("break", 0200, dbgdir, NULL, &brk_fops))
            err = -ENOMEM;
        if (!debugfs_create_file("populate", 0200, dbgdir, NULL, &populate_fops))
            err = -ENOMEM;
        if (!debugfs_create_file("clear", 0200, dbgdir, NULL, &clear_fops))
            err = -ENOMEM;
        if (!debugfs_create_file("thread", 0200, dbgdir, NULL, &thread_fops))
            err = -ENOMEM;
        if (!debugfs_create_file("timer", 0200, dbgdir, NULL, &timer_fops))
            err = -ENOMEM;
        if (!debugfs_create_file("stats", 0400, dbgdir, NULL, &stats_fops))
            err = -ENOMEM;
    }

    pr_info(DRV ": loaded%s\n", autobreak ? " (autobreak)" : "");
    if (autobreak)
        kgdb_breakpoint();

    return err;
}

static void __exit gdb_demo_exit(void)
{
    if (timer_running)
    {
        timer_running = false;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 9, 0)
        timer_shutdown_sync(&demo_timer);
#else
        del_timer_sync(&demo_timer);
#endif
    }
    if (demo_thread)
    {
        kthread_stop(demo_thread);
        demo_thread = NULL;
    }

    free_all_nodes();

    if (dbgdir)
        debugfs_remove_recursive(dbgdir);
    xa_destroy(&demo_xa);

    pr_info(DRV ": unloaded\n");
}

module_init(gdb_demo_init);
module_exit(gdb_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John O'Sullivan");
MODULE_DESCRIPTION("KGDB/KDB + GDB helpers demo module");
