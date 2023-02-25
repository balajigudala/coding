#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>

//static int fun_start(void)
//int init_module(void)
static int __init fun_start(void)
{
	pr_info("my module is : \n");
	pr_info("loaded succesfully \n");
	return 0;
}


//static void fun_end(void)
//void cleanup_module(void) 
static void __exit fun_end(void)
{
	printk(KERN_INFO "my module : \n");
	printk(KERN_INFO "unloaded succesfully\n");
}

module_init(fun_start)
module_exit(fun_end)

MODULE_AUTHOR("BALAJI");
MODULE_LICENSE("GPL_V2");
MODULE_DESCRIPTION("basic module");

