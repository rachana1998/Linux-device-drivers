#include<linux/module.h>
#include<linux/sched.h>
#include<linux/jiffies.h>

static int __init jif_init(void)
{
	unsigned long future_jiffy;
	
	future_jiffy=jiffies+HZ*5;     //got the future value of jiffie here
	printk("the current jiffy val is %lu  and future jiffy val is %lu\n",jiffies,future_jiffy);
/*the while loop checks the current jiffy values is less than future jiffy and if the condition is true it will return one and the loop will run */
/*and the loop condition will fail when the jiffies value is greater than future jiffies that is..the while loop runs only five seconds and after that the loop will failed */

/*if the schedule() is not used then the busyloop will run for five seconds*/

	while(time_before(jiffies,future_jiffy))
	{
	     		schedule();  		//it will stop the current process and called the new process to execute
						//after scheduling the control will check the condition in loop
	}
	printk("current jiffie is %lu and future jiffie val is %lu\n",jiffies,future_jiffy);
	return 0;
}
static void __exit jif_exit(void)
{
	printk("end\n");
}


module_init(jif_init);
module_exit(jif_exit);

