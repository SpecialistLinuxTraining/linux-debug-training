#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/of.h> // For device tree format specifier
#include <linux/mm.h> // For physical address demonstration
#include <asm/page.h> // For PAGE_SHIFT

#define pfn_to_phys(pfn) ((phys_addr_t)(pfn) << PAGE_SHIFT)

static int __init pr_format_demo_init(void)
{
    void *ptr = (void *)0xDEADBEEF;
    struct page *page = alloc_page(GFP_KERNEL);

    pr_info("[%s] Hashed pointer: %p\n", KBUILD_MODNAME, ptr);
    pr_info("[%s] Pointer as raw address: %px\n", KBUILD_MODNAME, ptr);
    pr_err("[%s] Error string for -EINVAL: %pe\n", KBUILD_MODNAME, ERR_PTR(-EINVAL));
    pr_warn("[%s] Function symbol: %pS\n", KBUILD_MODNAME, pr_format_demo_init);
    if (page)
    {
        phys_addr_t phys_addr = pfn_to_phys(page_to_pfn(page));
        pr_notice("[%s] Physical address: %pa\n", KBUILD_MODNAME, &phys_addr);
        __free_pages(page, 0);
    }
    else
    {
        pr_alert("[%s] Failed to allocate memory page\n", KBUILD_MODNAME);
    }

    pr_alert("[%s] Kernel pointer (subject to kptr_restrict): %pK\n", KBUILD_MODNAME, &ptr);

#ifdef CONFIG_OF
#ifndef __x86_64__ // x86 does not support the concept of a device tree
    struct device_node *node = of_find_node_by_path("/");
    if (node)
    {
        pr_info("[%s] Device-tree node: %pOF\n", KBUILD_MODNAME, node);
        of_node_put(node);
    }
    else
    {
        pr_err("[%s] Device tree root node not found\n", KBUILD_MODNAME);
    }
#endif
#endif

    return 0;
}

static void __exit pr_format_demo_exit(void)
{
    pr_info("[%s] Module is unloading\n", KBUILD_MODNAME);
}

module_init(pr_format_demo_init);
module_exit(pr_format_demo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("John OSullivan");
MODULE_DESCRIPTION("Demonstration of printk format specifiers using pr_*()");
