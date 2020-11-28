#include<linux/module.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/circ_buf.h>
#include<linux/slab.h>
#define SIZE 16
#include<linux/uaccess.h>
#include<linux/sched.h>
#include<linux/wait.h>
#include "ioctlheader.h"

#define MAJORN 55
#define NMINOR 3


MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");

dev_t devnum;
dev_t tempdev;

struct mydevice {
	struct cdev _cdev;
	struct circ_buf cbuff; 
	wait_queue_head_t _waitq;   //the process will waits 
}_dev[NMINOR];

struct mydevice *_devp;

static int circ_open(struct inode *inodep, struct file *filep)
{
	_devp = container_of(inodep->i_cdev,struct mydevice,_cdev);
	filep->private_data= _devp;
        printk("file is open\n");
        return 0;
}
static ssize_t circ_read(struct file *filep, char __user *ubuff , size_t cnt, loff_t *offset)
{
        int i;
       	int ret,mini;
        wait_event_interruptible(_devp->_waitq,CIRC_CNT(_devp->cbuff.head,_devp->cbuff.tail,SIZE)>0);

	mini =min(cnt,(size_t)CIRC_CNT(_devp->cbuff.head,_devp->cbuff.tail,SIZE));

        printk("size from user space for Read:%d\n",(int)cnt);
        for(i=0;i<mini;i++)
        {
                ret = copy_to_user(ubuff+i,_devp->cbuff.buf+_devp->cbuff.tail,1);
                        if(ret)
                        {
                                printk("Error in copying to the user\n");
                                return -EFAULT;
                        }
                printk("copied %c to user\n",ubuff[i]);
                _devp->cbuff.tail=(_devp->cbuff.tail+1)&(SIZE-1);
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
                ret=copy_from_user(_devp->cbuff.buf+_devp->cbuff.head,ubuff+i,1);
                if(ret){
                        printk("Error copying\n");
                        return -EFAULT;
                }
                printk("copied from user %c\t",_devp->cbuff.buf[_devp->cbuff.head]);
                _devp->cbuff.head = (_devp->cbuff.head+1)&(SIZE-1);
        }
	
	wake_up(&_devp->_waitq);
        printk("got data from user\n");
        return i;
}


static int circ_close(struct inode *inodep , struct file *filep)
{
        printk("file is closed\n");
        return 0;
}

long circ_ioctl(struct file *filep, unsigned int cmd,unsigned long arg)
{
	switch(cmd){
		case CMD_1:
			printk("this is the ioctl CMD_1\n");
			break;
		case CMD_2:
			printk("this is the ioctl CMD_2\n");
			break;
		default:
			printk("not supported\n");
			return -ENOTTY;
	}
	return 0;
}

struct file_operations fops={
        .open = circ_open,
        .read = circ_read,
        .write= circ_write,
        .release= circ_close,
	.unlocked_ioctl=circ_ioctl,
};


static int __init sample_init(void)
{	
	int i,j;
        int ret;
        devnum = MKDEV(MAJORN,0);
        ret = register_chrdev_region(devnum,NMINOR,"mywaitqdevice");
       	if(ret)
        {
                printk("unable to add the device number\n");
        	goto reg_out;
	
	}


	for(i=0;i<NMINOR;i++)
	{
		cdev_init(&_dev[i]._cdev,&fops);
               	 _dev[i].cbuff.buf = kmalloc(SIZE,GFP_KERNEL); //alocates memory to circ buffer
        if(!_dev[i].cbuff.buf)
        {
                printk("unable to allocate memory\n");
		goto cbuff_out;
        }
	init_waitqueue_head(&_dev[i]._waitq);
	}
	for(j=0;j<NMINOR;j++)            
	{
	tempdev= MKDEV(MAJOR(devnum),MINOR(devnum)+j);
	
        ret = cdev_add(&_dev[j]._cdev,tempdev,1);
        if(ret)
        {
                printk("unable to add the device to the kernel\n");
        	goto cdev_out;
	}
	}
        printk("Done init\n");
	return 0;
	
cdev_out:
        for(--j;j>=0;j--)
        cdev_del(&_dev[j]._cdev);


cbuff_out:
	for(--i;i>=0;i--)
	kfree(_dev[i].cbuff.buf);

reg_out:
	unregister_chrdev_region(devnum,NMINOR);
	return -1;

}
static  void __exit sample_exit(void)
{
	int i;
	for(i=0;i<NMINOR;i++)
	{
        cdev_del(&_dev[i]._cdev);
        kfree(_dev[i].cbuff.buf);
	}
        unregister_chrdev_region(devnum,NMINOR);
        printk("good bye\n");
}

module_init(sample_init);
module_exit(sample_exit);


