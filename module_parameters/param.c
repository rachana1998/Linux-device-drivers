#include<linux/param.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>


int intvar =10;
char *charvar="rachana";


static int __init hello_init(void)
{
	printk("module parameter %s\n",charvar);
        printk("hello world\n");
        printk("Done init\n");
        return 0;
}

static void __exit hello_exit(void)
{
printk("good bye\n");
}

module_param(intvar,int,S_IRUGO);
module_param(charvar,charp,S_IRUGO);

module_init(hello_init);
module_exit(hello_exit);


