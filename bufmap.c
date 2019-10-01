#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/slab.h>

static int bufmap_open(struct inode *inode, struct file*file)
{
	pr_info("I am open.\n");
	return 0;
}

static int bufmap_close(struct inode *inodep, struct file *filp)
{
	pr_info("I am closed.\n");
	return 0;
}

static long unlocked_ioctl(struct file *filp,unsigned int cmd, unsigned long arg)
{
	struct foo {
		int len;
		void *addr;
	} buffer;
	unsigned long ret, pfn;
	struct page **pages;
	long retval;
	int i;

	pr_info("ioctl: number %d\n", cmd);

	ret = copy_from_user(&buffer, (const void *) arg, sizeof(struct foo));
	pr_info("Size - %d, addr - 0x%p.\n", buffer.len, buffer.addr);

	pages = kmalloc(sizeof(struct page *)*buffer.len/4096, GFP_KERNEL);

	retval = get_user_pages_fast((unsigned long)buffer.addr, buffer.len/4096, 1, pages);

	pr_info("Number of pages = %ld\n", retval);

	for (i=0; i<retval; i++) {
		pfn = page_to_pfn(pages[i]);
		pr_info("Page %d, hwaddr=%lld\n", i, page_to_phys(pages[i]));
	}
	
	kfree(pages);

	return 0;
}

static const struct file_operations bufmap_fops = {
	.owner		= THIS_MODULE,
	.open		= bufmap_open,
	.release	= bufmap_close,
	.unlocked_ioctl = unlocked_ioctl,
};

struct miscdevice bufmap_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "bufmap_misc",
	.fops = &bufmap_fops,
};

static int __init bufmap_init(void)
{
	pr_info("bufmap loaded at 0x%p\n", bufmap_init);

	misc_register(&bufmap_device);
	return 0;
}

static void __exit bufmap_exit(void)
{
	misc_deregister(&bufmap_device);
	pr_info("bufmap says farewell!\n");
}

module_init(bufmap_init);
module_exit(bufmap_exit);

MODULE_AUTHOR("Philip Balister");
MODULE_LICENSE("GPL v2");

