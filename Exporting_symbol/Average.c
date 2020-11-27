#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>

static int mysum(int a, int b);
int avg,s;
int a=30,b=20;

MODULE_LICENSE("GPL");

static int __init average_init(void)
{
	printk("in average init\n");
	s= mysum(a,b);
	avg = s/2;
	
	printk("average is %d\n",avg);
	return 0;
}


static void __exit average_exit(void)
{
	printk("good bye\n");
}

module_init(average_init);
module_exit(average_exit);


