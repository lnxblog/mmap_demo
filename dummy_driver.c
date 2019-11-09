#include <linux/fs.h>
#include <linux/io.h>
#include <linux/memblock.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <asm/uaccess.h>

#define DEVICE_LEN 4096 
char *ptr;
static struct miscdevice misc;


static int dummy_fop_open(struct inode *inode, struct file *filp)
{
    printk(KERN_EMERG "dummy device opened\n");
    return 0;
}


static int dummy_fop_release(struct inode *inode, struct file *filp)
{
    printk(KERN_EMERG "dummy device closed / released\n");
    return 0;
}

static ssize_t dummy_fop_read(
	struct file *filp, char __user *data, size_t size, loff_t *offset)
{
	int bytes_read = 0;
	char *temp_ptr;
	temp_ptr = ptr;
	printk(KERN_EMERG "dummy device read\n");

	if (*temp_ptr == 0)
		return 0;

	while (size && *temp_ptr)
	{
		put_user(*(temp_ptr++), data++);
		size--;
		bytes_read++;
	}
	return bytes_read;
}

static ssize_t dummy_fop_mmap(struct file *filp, struct vm_area_struct *vma)
{
	long addr=ptr;
	if (remap_pfn_range(vma, vma->vm_start,virt_to_phys(addr)>>PAGE_SHIFT,DEVICE_LEN,vma->vm_page_prot))
	{
		printk("dummy device mmap error");
		return -EAGAIN;
	}

	return 0;

}



static const struct file_operations dummy_fops = {
	.owner = THIS_MODULE,
	.open = dummy_fop_open,
	.release = dummy_fop_release,
	.read = dummy_fop_read,
	.write = NULL,
	.mmap = dummy_fop_mmap
};

int __init initfunction(void)
{
	int ret;
	printk(KERN_EMERG "dummy device driver");
	ptr = kmalloc(DEVICE_LEN,GFP_KERNEL);
	if (!ptr)
	{
		printk(KERN_EMERG "memory allocation failed");
		return -ENOMEM;
	}
	strcpy(ptr,"hello world\n");  

	misc.minor = MISC_DYNAMIC_MINOR;
	misc.name = "dummy";
	misc.fops = &dummy_fops;

	ret = misc_register(&misc);

	if (ret) {
		printk(KERN_EMERG "dummy: failed to register device %s\n",
			misc.name);
		return -1;
	}
	return 0;
}
int __exit exitfunction(void)
{
	misc_deregister(&misc);
	printk(KERN_ALERT "dummy driver exit");

}
module_init(initfunction);
module_exit(exitfunction);
