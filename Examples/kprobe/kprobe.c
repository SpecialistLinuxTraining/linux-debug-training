#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

MODULE_LICENSE("GPL");
static struct kprobe kp;

int pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	pr_info("[%s] <%s> p->addr = 0x%p, ip = %lx, flags = 0x%lx\n",
			 __func__, p->symbol_name, p->addr,
			regs->ip, regs->flags);

	return 0;
}

void post_handler(struct kprobe *p, struct pt_regs *regs, unsigned long flags)
{
	pr_info("[%s] <%s> p->addr = 0x%p, flags = 0x%lx\n",
			__func__, p->symbol_name, p->addr,
		       	regs->flags);
}

static int kprobe_init(void)
{
	int ret;
	pr_info("%s: Initialising kprobe\n", __func__);
	kp.pre_handler = pre_handler;
	kp.post_handler = post_handler;
	kp.symbol_name = "kernel_clone";
	ret = register_kprobe(&kp);
	if (ret < 0) {
		pr_err("Failed to register kprobe, error %d\n", ret);
		return ret;
	}
	pr_info("Planted kprobe at %p\n", kp.addr);

	return 0;
}

static void kprobe_exit(void)
{
	unregister_kprobe(&kp);

	pr_info("%s: Exit kprobe\n", __func__);
}

module_init(kprobe_init);
module_exit(kprobe_exit);
