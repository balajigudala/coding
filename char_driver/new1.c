#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/ioport.h>
#include<linux/poll.h>
#include<linux/proc_fs.h>
#include<linux/errno.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/device.h>


dev_t devno;

//declaration of fuvctions
static int mychar_open(struct inode *inode,struct file *file);
static int mychar_close(struct inode *inode, struct file *file);
static ssize_t mychar_write(struct file *file, const char *buff, size_t len,loff_t *offset);
static ssize_t mychar_read(struct file *file,char *buff, size_t len,loff_t *offset);

wait_queue_head_t rwq,wwq;
struct proc_dir_entry *proc;

//DECLARE_WAIT_QUEUE_HEAD(rwq);
//DECLARE_WAIT_QUEUE_HEAD(wwq);
int n,a,uc;
#define size 5
struct class *ptr;
struct device *dptr;

// file opeartions structure
static struct file_operations fops={ .owner =THIS_MODULE , .open=mychar_open , .read=mychar_read , .write=mychar_write , .release=mychar_close};


// device context info structure
struct d_context
{
	char buf[1024];
	int no_chars;
	int base_address;
	int IRQ_lineno;
	struct cdev c_dev;
}my_context;


static int mychar_driver(void)
//int init_module(void)
//static int __init fun_start(void)
{
	int res=0;
	pr_info("my char driver is : \n");
	pr_info("loaded succesfully \n");
	//devno=MKDEV(42,0);
//	pr_info("dev no : %d\n",devno);
//	pr_info("major no : %d\n",MAJOR(devno));
//	pr_info("minor no : %d\n",MINOR(devno));
//	res=register_chrdev_region(devno,2,"mydriver");
	res=alloc_chrdev_region(&devno,0,2,"mydriver");
	if(res<0)
	{
		pr_info("my driver not registered \n");
		return -1;
	}
	else
	{
		pr_info("my driver registered \n");
	}


	pr_emerg("dev no :0 emerg");
	pr_alert("dev no :1 e");
	pr_crit("dev no :2 emerg");
	pr_err("dev no :3 emerg");
	pr_warn("dev no :4 emerg");
	pr_notice("dev no :5 emerg");
	pr_info("dev no :6 emerg");
	pr_debug("dev no :7 emerg");
	pr_info("dev no : %d\n",devno);
	pr_info("major no : %d\n",MAJOR(devno));
	pr_info("minor no : %d\n",MINOR(devno));
	cdev_init(&my_context.c_dev ,&fops);
	my_context.c_dev.owner=THIS_MODULE;
	res=cdev_add(&my_context.c_dev , devno ,2);
	if(res < 0)
	{
		printk(KERN_INFO"fail to add to kernel\n");
		unregister_chrdev_region(devno , 1);
		return -1;
	}
	init_waitqueue_head(&wwq);
	init_waitqueue_head(&rwq);
	printk(KERN_INFO"added sucessfully to kernel\n");
	ptr=class_create(THIS_MODULE,"mydev");
	dptr=device_create(ptr,NULL,devno,NULL,"mydev1");

	proc=proc_mkdir("balu_dir",NULL);
	proc_create("balu_file",0777,proc,NULL);
	return 0;
}


static void mychar_exit(void)
//void cleanup_module(void) 
//static void __exit fun_end(void)
{
	dev_t devno;
	devno=MKDEV(240,0);
	unregister_chrdev_region(devno,2);
	printk(KERN_INFO "my char driver : \n");
	printk(KERN_INFO "unloaded succesfully\n");
	device_destroy(ptr,devno);
	class_destroy(ptr);
}

module_init(mychar_driver);
module_exit(mychar_exit);

MODULE_AUTHOR("BALAJI");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("basic module");


// my char driver open  
static int mychar_open(struct inode *inode, struct file *file)
{
	try_module_get(THIS_MODULE);
	file->private_data=&my_context;
	if(file->f_mode & FMODE_READ)
		pr_info("open for read module \n");
	if(file->f_mode & FMODE_WRITE)
		pr_info("open for write module \n");
	return 0;
}
 
//my char driver close function
static int mychar_close(struct inode *inode, struct file *file)
{
	//module_put(THIS_MODULE);
	dev_t devno;
	devno= MKDEV(42,0);
	module_put(THIS_MODULE);
	pr_info("my driver closed\n");
	proc_remove(proc);
	return 0;
}

/*//my char driver write function
static ssize_t mychar_write(struct file *file, const char *buff, size_t len,loff_t *offset)
{
	struct d_context *tdev;
	tdev=file->private_data;
	if(len>1024)
		len=1024;

        pr_info("read is invoked \n");
	copy_from_user(tdev->buf,buff,len);
	pr_info("in write routine : %s \n",tdev->buf);
	tdev->no_chars=len;
	pr_info("write is invoked \n");
	return (ssize_t)len;
}*/


static ssize_t mychar_write(struct file *file, const char *buff, size_t len,loff_t *offset)
{
	struct d_context *tdev;
	//wait_event_interruptible(wwq,n<size);
	wait_event_interruptible(wwq,n<size);
	tdev=file->private_data;
	a=size-n;
	if(len>0)
		len=a;
	copy_from_user(tdev->buf,buff,len);
	n=n+len;
	pr_info("write is invoked \n");
	pr_info("in write routine : %s \n",tdev->buf);
	//wake_up_interruptible(&rwq);
	wake_up_interruptible(&rwq);
	return len;
}

//my char driver read function 
/*static ssize_t mychar_read(struct file *file,char *buff, size_t len,loff_t *offset)
{
	struct d_context *tdev;
	tdev=file->private_data;
	if(len>tdev->no_chars)
		len=tdev->no_chars;
	copy_to_user(buff,tdev->buf,len);

	tdev->no_chars=0;
	pr_info("read is invoked \n");
	return (ssize_t)len;
}*/


static ssize_t mychar_read(struct file *file,char *buff, size_t len,loff_t *offset)
{
	struct d_context *tdev;
	tdev=file->private_data;
	//wait_event_interruptible(rwq,n>0);
	wait_event_interruptible(rwq,n>0);
	if(n<len)
		uc=len;
	copy_to_user(buff,tdev->buf,uc);
	n=n-uc;
	//wake_up_interruptible(&wwq);
	wake_up_interruptible(&wwq);
	pr_info("read is invoked \n");
	return uc;
}
