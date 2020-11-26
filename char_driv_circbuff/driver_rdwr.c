#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");

dev_t devnum;
struct cdev _cdev;

static int sample_open(struct inode *inode,struct file *filep)
{
	printk(" sample open \n");
	return 0;
}

static ssize_t sample_write(struct file *filep, const char __user *ubuff, size_t cnt, loff_t *offset)
{
	printk("sample write\n");
	return 0;
}
static ssize_t sample_read(struct file *filep, char __user *ubuff, size_t cnt, loff_t *offset)
{
	printk("sample read\n");
	return 0;
}

static int sample_close(struct inode *inode,struct file *filep)
{
	printk(" sample close \n");
	return 0;
}

struct file_operations fops={
	.open    = sample_open,
	.write    = sample_write,
	.read   = sample_read,
	.release = sample_close,
};

static int __init sample_init(void)
{
	int ret;
	devnum = MKDEV(42,0);
	ret = register_chrdev_region(devnum,1,"sample_dev");
	if(ret)
	{
		printk("kernel didn't grant us a device number\n");
		return ret;
	}
	
	cdev_init(&_cdev,&fops);
	ret = cdev_add(&_cdev,devnum,1);
	if(ret)
	{
		printk("device is not added to kernel\n");
		unregister_chrdev_region(devnum,1);
		return ret;
	}

	printk("Done init\n");
	return 0;
}

static void __exit sample_exit(void)
{
	cdev_del(&_cdev);
	unregister_chrdev_region(devnum,1);
	printk("good bye\n");
}

module_init(sample_init);
module_exit(sample_exit);

