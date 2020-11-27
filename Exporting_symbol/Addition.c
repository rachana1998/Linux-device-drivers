#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

static int mysum(int a ,int b);
MODULE_LICENSE("GPL");

static int mysum(int a, int b)
{
	return a+b;
}

static int __init hello_init(void)
{
	printk("in init module\n");
	printk("Done init\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk("good bye\n");
}

EXPORT_SYMBOL_GPL(mysum);

module_init(hello_init);
module_exit(hello_exit);


