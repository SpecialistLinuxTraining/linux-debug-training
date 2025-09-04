// SPDX-License-Identifier: GPL-2.0
//
// kcsan_race_demo.c - Intentionally racy demo for KCSAN with debugfs controls.
// Shared resource is updated by multiple writers (kthread + on-demand "blast"),
// and read by a periodic timer callback; no synchronization is used.
//
// Build with CONFIG_KCSAN=y to get "KCSAN: data-race" reports in dmesg.
//
// Debugfs directory: /sys/kernel/debug/kcsan_race_demo
//
// Files:
//   thread           : "start" / "stop"
//   timer            : "start" / "stop"
//   blast            : "<N>"   (run N racy write iterations now)
//   stats            : read-only summary + snapshot
//   reset            : write anything to reset stats and state
//   write_delay_us   : "<us>"  (writer thread delay per iteration)
//   blast_delay_us   : "<us>"  (blast writer delay per iteration)
//   timer_spin_us    : "<us>"  (udelay in timer; increases overlap)
//
// Notes:
// - Intentionally omits locks and atomics on the shared resource.
// - Stats counters use atomics to avoid drowning KCSAN in noise on stats.
// - Safe by design: no frees or OOB writes; only logical invariant breakage.

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/version.h>
#include <linux/smp.h>

#define DRV "kcsan_race_demo"

/* ---------------- Shared resource (intentionally unsynchronized) ----------- */

struct shared_resource
{
    /* Writer sets valid=false, updates fields, then sets valid=true.
     * Reader assumes invariants hold when valid==true, but there is no
     * synchronization so it can observe partial updates.
     */
    bool valid;
    u32 len;      /* 0..sizeof(buf)-1 */
    u64 seq;      /* increases every write */
    u32 checksum; /* sum of buf[0..len-1] */
    char buf[64]; /* payload + trailing '\0' when valid */
};

/* This instance is the intentionally racy object. */
static struct shared_resource R;

/* ---------------- Tuning knobs and stats (stats use atomics) --------------- */

static struct task_struct *writer_task;
static struct timer_list race_timer;
static bool timer_running;

static atomic64_t writes_thread;
static atomic64_t writes_blast;
static atomic64_t reads_timer;
static atomic64_t mismatch_count;

static int write_delay_us = 50;
static int blast_delay_us;
static int timer_spin_us; /* busy wait inside timer; default 0 */

module_param(write_delay_us, int, 0644);
MODULE_PARM_DESC(write_delay_us, "udelay() in writer thread per iteration");

module_param(blast_delay_us, int, 0644);
MODULE_PARM_DESC(blast_delay_us, "udelay() in each blast iteration");

module_param(timer_spin_us, int, 0644);
MODULE_PARM_DESC(timer_spin_us, "udelay() inside timer (softirq); use sparingly");

/* ---------------- Helpers -------------------------------------------------- */

static inline u32 buf_checksum(const char *p, u32 n)
{
    u32 s = 0;
    u32 i;
    for (i = 0; i < n; i++)
        s += (u8)p[i];
    return s;
}

/* Prepare a new pattern based on seq. Returns pattern byte and len. */
static inline void fill_pattern(struct shared_resource *r, u64 seq)
{
    /* pattern cycles A..Z to make mismatches obvious */
    u8 pat = 'A' + (seq % 26);
    u32 new_len = (seq % (sizeof(r->buf) - 1u)) + 1u; /* 1..63 */

    /* No synchronization on R at all, by design. */
    r->valid = false;

    r->seq = seq;
    r->len = new_len;

    /* Deliberately store payload first then checksum and valid.
     * Reader may see: valid=true with old/new mix.
     */
    memset(r->buf, pat, new_len);
    r->buf[new_len] = '\0';

    r->checksum = buf_checksum(r->buf, new_len);

    /* Publish as "valid", but there is no barrier to pair with readers. */
    r->valid = true;
}

/* Validate invariants the writer "intended" to publish. Return true if ok. */
static bool check_invariants(struct shared_resource *r, u64 *out_seq,
                             u32 *out_len, u8 *out_pat)
{
    /* Snapshot fields without any READ_ONCE or barriers (on purpose). */
    bool valid = r->valid;
    u64 seq = r->seq;
    u32 len = r->len;
    u32 csum = r->checksum;

    /* The intended pattern byte derived from seq. */
    u8 pat = 'A' + (seq % 26);
    bool ok = true;

    if (!valid)
        goto out;

    /* Bounds check len defensively to avoid OOB if torn read occurs. */
    if (len >= sizeof(r->buf))
        len = sizeof(r->buf) - 1;

    /* Check contents match pattern and NUL terminator is in place. */
    {
        u32 i;
        for (i = 0; i < len; i++)
        {
            if ((u8)r->buf[i] != pat)
            {
                ok = false;
                break;
            }
        }
        if (r->buf[len] != '\0')
            ok = false;
    }

    /* Check checksum matches what we observe now. */
    if (buf_checksum(r->buf, len) != csum)
        ok = false;

out:
    if (out_seq)
        *out_seq = seq;
    if (out_len)
        *out_len = len;
    if (out_pat)
        *out_pat = pat;

    return ok && valid;
}

/* ---------------- Worker contexts ----------------------------------------- */

static int writer_thread_fn(void *arg)
{
    u64 local_seq = 1;

    pr_info(DRV ": writer thread started on CPU %d\n", smp_processor_id());

    while (!kthread_should_stop())
    {
        fill_pattern(&R, local_seq++);
        atomic64_inc(&writes_thread);

        if (write_delay_us > 0)
            udelay(write_delay_us);
        cpu_relax();
    }

    pr_info(DRV ": writer thread stopping\n");
    return 0;
}

static void race_timer_fn(struct timer_list *t)
{
    bool ok;
    u64 seq;
    u32 len;
    u8 pat;

    if (timer_spin_us > 0)
        udelay(timer_spin_us);

    ok = check_invariants(&R, &seq, &len, &pat);
    atomic64_inc(&reads_timer);
    if (!ok)
        atomic64_inc(&mismatch_count);

    if (timer_running)
        mod_timer(&race_timer, jiffies + HZ / 100); /* 10ms period */
}

/* ---------------- Debugfs: helpers ----------------------------------------- */

static struct dentry *dbgdir;

static ssize_t write_string_cmd(struct file *f, const char __user *ubuf,
                                size_t len, loff_t *ppos,
                                int (*handler)(const char *))
{
    char buf[32];

    if (len == 0 || len >= sizeof(buf))
        return -EINVAL;
    if (copy_from_user(buf, ubuf, len))
        return -EFAULT;
    buf[len] = '\0';
    if (buf[len - 1] == '\n')
        buf[len - 1] = '\0';

    if (handler)
        handler(buf);

    return len;
}

static int cmd_thread(const char *s)
{
    if (sysfs_streq(s, "start"))
    {
        if (!writer_task)
        {
            struct task_struct *tsk =
                kthread_run(writer_thread_fn, NULL, DRV "/writer");
            if (IS_ERR(tsk))
            {
                pr_err(DRV ": kthread_run failed: %ld\n",
                       PTR_ERR(tsk));
                return -ECHILD;
            }
            writer_task = tsk;
        }
    }
    else if (sysfs_streq(s, "stop"))
    {
        if (writer_task)
        {
            kthread_stop(writer_task);
            writer_task = NULL;
        }
    }
    return 0;
}

static int cmd_timer(const char *s)
{
    if (sysfs_streq(s, "start"))
    {
        if (!timer_running)
        {
            timer_running = true;
            mod_timer(&race_timer, jiffies + HZ / 50); /* 20ms */
        }
    }
    else if (sysfs_streq(s, "stop"))
    {
        if (timer_running)
        {
            timer_running = false;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 9, 0)
            timer_shutdown_sync(&race_timer);
#else
            del_timer_sync(&race_timer);
#endif
        }
    }
    return 0;
}

static int parse_int(const char *s, int *out)
{
    long v;
    if (kstrtol(s, 0, &v))
        return -EINVAL;
    *out = (int)v;
    return 0;
}

static int cmd_blast(const char *s)
{
    long n;
    u64 seq_base;
    if (kstrtol(s, 0, &n) || n <= 0)
        return -EINVAL;

    /* Derive a starting seq to keep pattern evolving. */
    seq_base = R.seq + 1;

    while (n--)
    {
        fill_pattern(&R, seq_base++);
        atomic64_inc(&writes_blast);
        if (blast_delay_us > 0)
            udelay(blast_delay_us);
        cpu_relax();
    }
    return 0;
}

static int cmd_reset(const char *s)
{
    atomic64_set(&writes_thread, 0);
    atomic64_set(&writes_blast, 0);
    atomic64_set(&reads_timer, 0);
    atomic64_set(&mismatch_count, 0);

    /* Reinitialize resource deterministically. */
    memset(&R, 0, sizeof(R));
    R.valid = false;
    R.seq = 0;
    R.len = 0;
    R.buf[0] = '\0';
    R.checksum = 0;

    return 0;
}

/* ---------------- Debugfs fops --------------------------------------------- */

static ssize_t thread_write(struct file *f, const char __user *ubuf,
                            size_t len, loff_t *ppos)
{
    return write_string_cmd(f, ubuf, len, ppos, cmd_thread);
}
static const struct file_operations thread_fops = {
    .write = thread_write,
    .llseek = noop_llseek,
};

static ssize_t timer_write(struct file *f, const char __user *ubuf,
                           size_t len, loff_t *ppos)
{
    return write_string_cmd(f, ubuf, len, ppos, cmd_timer);
}
static const struct file_operations timer_fops = {
    .write = timer_write,
    .llseek = noop_llseek,
};

static ssize_t blast_write(struct file *f, const char __user *ubuf,
                           size_t len, loff_t *ppos)
{
    return write_string_cmd(f, ubuf, len, ppos, cmd_blast);
}
static const struct file_operations blast_fops = {
    .write = blast_write,
    .llseek = noop_llseek,
};

static ssize_t reset_write(struct file *f, const char __user *ubuf,
                           size_t len, loff_t *ppos)
{
    return write_string_cmd(f, ubuf, len, ppos, cmd_reset);
}
static const struct file_operations reset_fops = {
    .write = reset_write,
    .llseek = noop_llseek,
};

static int stats_show(struct seq_file *m, void *v)
{
    /* Snapshot without synchronization, to keep it honest. */
    struct shared_resource S = R;

    seq_printf(m, "writes_thread=%lld\n",
               (long long)atomic64_read(&writes_thread));
    seq_printf(m, "writes_blast=%lld\n",
               (long long)atomic64_read(&writes_blast));
    seq_printf(m, "reads_timer=%lld\n",
               (long long)atomic64_read(&reads_timer));
    seq_printf(m, "mismatch_count=%lld\n",
               (long long)atomic64_read(&mismatch_count));

    seq_puts(m, "--- resource snapshot (racy) ---\n");
    seq_printf(m, "valid=%u\n", S.valid ? 1 : 0);
    seq_printf(m, "seq=%llu\n", (unsigned long long)S.seq);
    seq_printf(m, "len=%u\n", S.len);
    seq_printf(m, "checksum=%u\n", S.checksum);
    seq_printf(m, "buf[0..min(len,16))=");
    {
        u32 n = S.len < 16 ? S.len : 16;
        u32 i;
        for (i = 0; i < n; i++)
            seq_printf(m, "%02x ", (u8)S.buf[i]);
        seq_puts(m, "\n");
    }
    return 0;
}

static int stats_open(struct inode *inode, struct file *file)
{
    return single_open(file, stats_show, inode->i_private);
}
static const struct file_operations stats_fops = {
    .open = stats_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static ssize_t write_delay_write(struct file *f, const char __user *ubuf,
                                 size_t len, loff_t *ppos)
{
    return write_string_cmd(f, ubuf, len, ppos, ({
                                int __fn(const char *s) { return parse_int(s, &write_delay_us); }
                                __fn;
                            }));
}
static const struct file_operations write_delay_fops = {
    .write = write_delay_write,
    .llseek = noop_llseek,
};

static ssize_t blast_delay_write(struct file *f, const char __user *ubuf,
                                 size_t len, loff_t *ppos)
{
    return write_string_cmd(f, ubuf, len, ppos, ({
                                int __fn(const char *s) { return parse_int(s, &blast_delay_us); }
                                __fn;
                            }));
}
static const struct file_operations blast_delay_fops = {
    .write = blast_delay_write,
    .llseek = noop_llseek,
};

static ssize_t timer_spin_write(struct file *f, const char __user *ubuf,
                                size_t len, loff_t *ppos)
{
    return write_string_cmd(f, ubuf, len, ppos, ({
                                int __fn(const char *s) { return parse_int(s, &timer_spin_us); }
                                __fn;
                            }));
}
static const struct file_operations timer_spin_fops = {
    .write = timer_spin_write,
    .llseek = noop_llseek,
};

/* ---------------- Module init/exit ----------------------------------------- */

static int __init kcsan_race_demo_init(void)
{
    int err = 0;

    memset(&R, 0, sizeof(R));
    atomic64_set(&writes_thread, 0);
    atomic64_set(&writes_blast, 0);
    atomic64_set(&reads_timer, 0);
    atomic64_set(&mismatch_count, 0);

    timer_running = false;
    timer_setup(&race_timer, race_timer_fn, 0);

    dbgdir = debugfs_create_dir(DRV, NULL);
    if (!dbgdir)
    {
        pr_warn(DRV ": debugfs not available; continuing without controls\n");
        goto out;
    }

    if (!debugfs_create_file("thread", 0200, dbgdir, NULL, &thread_fops))
        err = -ENOMEM;
    if (!debugfs_create_file("timer", 0200, dbgdir, NULL, &timer_fops))
        err = -ENOMEM;
    if (!debugfs_create_file("blast", 0200, dbgdir, NULL, &blast_fops))
        err = -ENOMEM;
    if (!debugfs_create_file("reset", 0200, dbgdir, NULL, &reset_fops))
        err = -ENOMEM;
    if (!debugfs_create_file("stats", 0400, dbgdir, NULL, &stats_fops))
        err = -ENOMEM;

    if (!debugfs_create_file("write_delay_us", 0200, dbgdir, NULL, &write_delay_fops))
        err = -ENOMEM;
    if (!debugfs_create_file("blast_delay_us", 0200, dbgdir, NULL, &blast_delay_fops))
        err = -ENOMEM;
    if (!debugfs_create_file("timer_spin_us", 0200, dbgdir, NULL, &timer_spin_fops))
        err = -ENOMEM;

out:
    pr_info(DRV ": loaded (use debugfs to start thread/timer and blast writes)\n");
    return err;
}

static void __exit kcsan_race_demo_exit(void)
{
    if (timer_running)
    {
        timer_running = false;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 9, 0)
        timer_shutdown_sync(&race_timer);
#else
        del_timer_sync(&race_timer);
#endif
    }
    if (writer_task)
    {
        kthread_stop(writer_task);
        writer_task = NULL;
    }

    if (dbgdir)
        debugfs_remove_recursive(dbgdir);

    pr_info(DRV ": unloaded\n");
}

module_init(kcsan_race_demo_init);
module_exit(kcsan_race_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John O'Sullivan");
MODULE_DESCRIPTION("KCSAN race demonstration with kthread, timer, and debugfs");
MODULE_VERSION("1.0");
