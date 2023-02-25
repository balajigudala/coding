#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/ioport.h>
#include<linux/fs.h>
#include<linux/errno.h>
#include<linux/poll.h>
#include<linux/proc_fs.h>
#include<asm/io.h>
#include<linux/interrupt.h>
#include<linux/sched.h>

MODULE_LICSENSE("GPL");







#define MYDEV_MAJOR_NUM 42
#define MYDEV_NAME      "myserial"
#define SER_IQBASE      0x3f8
#define SER_IRQ         4

static struct file_operations ops =
{
	.read = myserial_read,
	.write= myserial_write,
	.open = myserial_open,
	.release=myserial_close
};

#define SUCCESS 0
#define MAXSIZE 4000
#define DATA
#

