// SPDX-License-Identifier: GPL-2.0
/* lockdep_demo.c – intentionally violates the A→B lock order */

#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/delay.h>

static DEFINE_MUTEX(lock_A);
static DEFINE_MUTEX(lock_B);

static struct task_struct *thr_a;
static struct task_struct *thr_b;

/* Thread 1: acquires A → B */
static int thread_a_fn(void *unused)
{
    while (!kthread_should_stop())
    {
        mutex_lock(&lock_A);
        msleep(20); /* give the other thread a chance */
        mutex_lock(&lock_B);

        mutex_unlock(&lock_B);
        mutex_unlock(&lock_A);
        msleep(20);
    }
    return 0;
}

/* Thread 2: acquires B → A (opposite order) */
static int thread_b_fn(void *unused)
{
    while (!kthread_should_stop())
    {
        mutex_lock(&lock_B);
        msleep(20);
        mutex_lock(&lock_A);

        mutex_unlock(&lock_A);
        mutex_unlock(&lock_B);
        msleep(20);
    }
    return 0;
}

static int __init lockdep_demo_init(void)
{
    pr_info("lockdep_demo: loading – expect a circular-dependency warning!\n");

    thr_a = kthread_run(thread_a_fn, NULL, "lockdep_demo/a");
    if (IS_ERR(thr_a))
        return PTR_ERR(thr_a);

    thr_b = kthread_run(thread_b_fn, NULL, "lockdep_demo/b");
    if (IS_ERR(thr_b))
    {
        kthread_stop(thr_a);
        return PTR_ERR(thr_b);
    }

    return 0;
}

static void __exit lockdep_demo_exit(void)
{
    kthread_stop(thr_a);
    kthread_stop(thr_b);
    pr_info("lockdep_demo: unloaded.\n");
}

module_init(lockdep_demo_init);
module_exit(lockdep_demo_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Tiny AB-BA deadlock demo for Lockdep");
MODULE_LICENSE("GPL");
