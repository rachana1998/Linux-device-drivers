#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/circ_buf.h>
#include<linux/slab.h>
#define SIZE 16
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");

dev_t devnum;
struct cdev _cdev;
struct circ_buf cbuff;

static int circ_open(struct inode *inodep, struct file *filep)
{
	printk("file is opened\n");
	return 0;
}
static ssize_t circ_read(struct file *filep, char __user *ubuff , size_t cnt, loff_t *offset)
{
	int i;
	int ret,mini;
	mini =min(cnt,(size_t)CIRC_CNT(cbuff.head,cbuff.tail,SIZE));

	printk("size from user space for Read:%d\n",(int)cnt);
	for(i=0;i<mini;i++)
	{
		ret = copy_to_user(ubuff+i,cbuff.buf+cbuff.tail,1);
			if(ret)
			{
				printk("Error in copying to the user\n");
				return -EFAULT;
			}
		printk("copied %c to user\n",ubuff[i]);
		cbuff.tail=(cbuff.tail+1)&(SIZE-1);
	}
	printk("circ read function is finished\n");
	return i;
}
static ssize_t circ_write(struct file *filep, const char __user *ubuff , size_t cnt , loff_t *offset)
{
	int ret,i;
	printk("size sent from user space is %d\n",(int)cnt);
	for(i=0;i<(int)cnt;i++)
	{
		ret=copy_from_user(cbuff.buf+cbuff.head,ubuff+i,1); 
		if(ret){
			printk("Error copying\n");
			return -EFAULT;
		}
		printk("copied from user %c\t",cbuff.buf[cbuff.head]);
		cbuff.head = (cbuff.head+1)&(SIZE-1);
	}
	printk("got data from user\n");
	return i;
}


static int circ_close(struct inode *inodep , struct file *filep)
{
	printk("file is closed\n");
	return 0;
}


struct file_operations fops={
	.open = circ_open,
	.read = circ_read,
	.write= circ_write,
	.release= circ_close,
};


static int __init circ_init(void)
{
	int ret;
	devnum = MKDEV(50,0);
	ret = register_chrdev_region(devnum,1,"mydevicecirc");
	if(ret)
	{
		printk("unable to add the device number\n");
		return ret;
	}
	cdev_init(&_cdev,&fops);
	cbuff.buf = kmalloc(SIZE,GFP_KERNEL); //alocates memory to circ buffer
	if(!cbuff.buf)
	{
		printk("unable to allocate memory\n");
		unregister_chrdev_region(devnum,1);
		return -1;
	}

	ret = cdev_add(&_cdev,devnum,1);
	if(ret)
	{
		printk("unable to add the device to the kernel\n");
		kfree(cbuff.buf);
		unregister_chrdev_region(devnum,1);
		return ret;
	}
	
	printk("Done init\n");
	return 0;
}

static void __exit circ_exit(void)
{
	cdev_del(&_cdev);
	kfree(cbuff.buf);
	unregister_chrdev_region(devnum,1);
	printk("good bye\n");
}

module_init(circ_init);
module_exit(circ_exit);

