#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
int MYDEV_MAJOR_NUM;

MODULE_LICENSE("GPL");

static int myserial_open(struct inode *inode, struct file *file);
static int myserial_close(struct inode *inode, struct file *file);
static ssize_t myserial_read(struct file *file, char *buf, size_t count,
                          loff_t *offset);
static ssize_t myserial_write(struct file *file, const char *buf, size_t count,
                           loff_t *offset);
static void Int_Receiv(void);
static void Int_Transmt(void);

#define MYDEV_MAJOR_NUM   42
#define MYDEV_NAME        "myserial"
#define SER_IOBASE        0x3f8
#define SER_IRQ		  4


static struct file_operations myserial_ops =
{ 
  .read    = myserial_read,
  .write   = myserial_write,
  .open    = myserial_open,
  .release = myserial_close,
};

#define  SUCCESS      0
#define  MAXSIZE      4000 
#define   DATA             0
#define   DIV_LATCH_LOW    0
#define   DIV_LATCH_HIGH   1
#define   IER              1
#define   IIR              2
#define   LCR              3
#define   MDMC             4
#define   LSR              5

#define SER_DEV "myserialDev"


/**** circular queue ****/
typedef struct CircularBuf
{
  int RdIndex;
  int WrIndex;
  int NoOfChar;
  char Buf[MAXSIZE];
}Cirq_t;

/***  control block structure ****/
typedef struct
{
  struct semaphore sem;
  wait_queue_head_t rxWq;
  wait_queue_head_t txWq;
  unsigned int baseAddr;
  int irq;
  Cirq_t Txq;
  Cirq_t Rxq;
}Cblock_t;

Cblock_t Cblkp;

/*******************************************************************************
* Name: Int_Receiv
* 
* Description:
*   This function receives characters from serial port and store in receive 
*   queue and give semaphore.
* Parameters :
*   Input : control block pointer.
* Returns :  nothing
*******************************************************************************/
static void Int_Receiv(void) /*RI */
{
  while(inb(Cblkp.baseAddr+LSR) & 0x1)/*inb= data from device to comp*/
  {
    if(Cblkp.Rxq.NoOfChar < MAXSIZE)
    {
        Cblkp.Rxq.Buf[Cblkp.Rxq.WrIndex] = inb(Cblkp.baseAddr+DATA);
        Cblkp.Rxq.WrIndex++;
        Cblkp.Rxq.NoOfChar++;
        if(Cblkp.Rxq.WrIndex == MAXSIZE)
            Cblkp.Rxq.WrIndex = 0;
    }
    else
    {
      inb(Cblkp.baseAddr+DATA);
    }
  }
  wake_up_interruptible(&Cblkp.rxWq);
}   


/*******************************************************************************
* Name: Int_Transmt
* Description:
*   This function is called when transmitt interrupt comes .
*   This function responsibility is taking characters transmit  queue and
*   transmitt to serial port.
* Parameters :
*   Input : device control block pointer.
* Returns :SER_DEV
*   nothingR
*******************************************************************************/
static void Int_Transmt(void)
{
  if(Cblkp.Txq.NoOfChar == 0)
  { 
    /* disable transmit interrupt */
    outb((inb(Cblkp.baseAddr+ IER) & 0xfd),Cblkp.baseAddr+ IER);
    return ;     
  }
  else
  { 
    
    while((inb(Cblkp.baseAddr+ LSR) & 0x20) && (Cblkp.Txq.NoOfChar)) /* verifying 5th bit of LSR && 														total no of chars ->0 */
    {
      outb(Cblkp.Txq.Buf[Cblkp.Txq.RdIndex++],Cblkp.baseAddr+DATA); /* Tx buf -> THR */
      Cblkp.Txq.NoOfChar--; /* decrementing total no of chars */   
      if(Cblkp.Txq.RdIndex == MAXSIZE)
         Cblkp.Txq.RdIndex = 0;
    }
    wake_up_interruptible(&Cblkp.txWq);
  }
}

/***********R********************************************************************
* Name:myIntHandler
* Description:
*******************************************************************************/
static irqreturn_t myIntHandler(int irq, void *dev_id)
{

  unsigned char intStat;
  

  intStat = inb(Cblkp.baseAddr+IIR);
  while((intStat & 1) == 0)/* interrupt is pending or not*/
  {

    if((intStat & 0x2) == 0x2)/* verifying 1st bit of IIR - TI */
      Int_Transmt();

    if((intStat & 0x4) == 0x4)/* verifying 2nd bit of IIR - RI */
      Int_Receiv();

    intStat = inb(Cblkp.baseAddr+IIR);
  }
  return IRQ_HANDLED;
}

static int serInit(int irq, unsigned int baseaddr)
{
  

  Cblkp.Rxq.RdIndex  = 0;
  Cblkp.Rxq.WrIndex  = 0;
  Cblkp.Rxq.NoOfChar = 0;
  Cblkp.Txq.RdIndex  = 0;
  Cblkp.Txq.WrIndex  = 0;
  Cblkp.Txq.NoOfChar = 0;

  Cblkp.baseAddr = baseaddr;
  Cblkp.irq = irq;

  sema_init(&Cblkp.sem,1);
  init_waitqueue_head(&Cblkp.rxWq);
  init_waitqueue_head(&Cblkp.txWq);

  /*** Set the baud rate ***/
  outb(0x83, Cblkp.baseAddr+LCR);
  outb(0x01, Cblkp.baseAddr+DIV_LATCH_LOW);
  outb(0x00, Cblkp.baseAddr+DIV_LATCH_HIGH);
  outb(0x03, Cblkp.baseAddr+LCR);
  outb(0x0b, Cblkp.baseAddr+MDMC);
  outb(0x0, Cblkp.baseAddr+IER);
 
  /*** Enable Fifos ***/
  outb(0x70, Cblkp.baseAddr+IIR);

  /** Install ISR for serial port **/
  //if(request_irq(irq, myIntHandler, SA_INTERRUPT, SER_DEV,NULL))
//if(request_irq(irq, myIntHandler, IRQF_DISABLED, SER_DEV,NULL))	
if(request_irq(irq, myIntHandler, 0, SER_DEV,NULL))
  {
    printk("<1>Can't get interrupt %d\n",irq);
    return -1;
  }
   return 0;
}

/*******************************************************************************
* Name:init_module
* Description:
*******************************************************************************/
int myserial_init(void)
{
  int res;
//res=alloc_chrdev_region(&MYDEV_MAJOR_NUM,0,2,"serial_driver");

res = register_chrdev(MYDEV_MAJOR_NUM, SER_DEV, &myserial_ops);
  if(res<0)
  {
    printk("<1>" "Registration Error %d\n",res);
    return res;
  }
  else
  {
    printk("<1>" "Registration success %d\n",res);
  }

  //ev_init(&
  if(!request_region(SER_IOBASE,8,SER_DEV))
  {
    printk(KERN_INFO "can't get I/O port address 0x3f8\n");
    return -1;
  }
 
  return serInit(SER_IRQ, SER_IOBASE);
}

/*******************************************************************************
* Name:cleanup_module
* Description:
*******************************************************************************/
void myserial_cleanup(void)
{
  unregister_chrdev(MYDEV_MAJOR_NUM, SER_DEV);
  free_irq(SER_IRQ,NULL);
  release_region(SER_IOBASE,8);
  printk(KERN_ALERT "good bye\n");

}

/*******************************************************************************
* Name:myserial_open
* Description:
*******************************************************************************/
static int myserial_open(struct inode *inode, struct file * file)
{
  try_module_get(THIS_MODULE);
  if (file->f_mode & FMODE_READ)
    printk("<1>" "open for read\n");
  if (file->f_mode & FMODE_WRITE)
    printk("<1>" "opened for write\n");

  /*** Enable interrupts on serial port 1 */
  outb(1, Cblkp.baseAddr+IER);
  
  return 0;	
}


/*******************************************************************************
* Name:myserial_close
* Description:
*******************************************************************************/
static int myserial_close(struct inode *inode, struct file * file)
{
  module_put(THIS_MODULE);
  printk("<1>" "device closed\n");
  /*** Disable Receive interrupt */
  outb((inb(Cblkp.baseAddr+ IER) & 0xfe),Cblkp.baseAddr+ IER);
  return 0;
}


/*******************************************************************************
* Name:myserial_read
* Description:
*******************************************************************************/
static ssize_t myserial_read(struct file *file, char * buf,
                          size_t len,  loff_t * offset)
{
	
  int cLen;
  int partlen;

  printk("<5>" "read called \n");
  wait_event_interruptible(Cblkp.rxWq, Cblkp.Rxq.NoOfChar > 0);
  if(len < Cblkp.Rxq.NoOfChar)
    cLen = len;
  else
    cLen = Cblkp.Rxq.NoOfChar;

  if((Cblkp.Rxq.RdIndex + cLen) <= MAXSIZE)
  {
    copy_to_user(buf, Cblkp.Rxq.Buf+Cblkp.Rxq.RdIndex, cLen);
  }
  else
  {
    partlen = MAXSIZE - Cblkp.Rxq.RdIndex;
    copy_to_user(buf, Cblkp.Rxq.Buf+Cblkp.Rxq.RdIndex, partlen);
    copy_to_user(buf+partlen, Cblkp.Rxq.Buf, cLen - partlen);
  }
  Cblkp.Rxq.RdIndex =  (Cblkp.Rxq.RdIndex + cLen) % MAXSIZE;
  Cblkp.Rxq.NoOfChar  -= cLen;
  return(cLen);

}

/*******************************************************************************
* Name:myserial_write
* Description:
*******************************************************************************/
static ssize_t myserial_write(struct file * file, const char * buf,
                       size_t count,loff_t * offset)
{
  int avllen, copylen, partlen;
  int culen = count;

  printk("<1>" "write called \n");


  while(culen)
  {
    wait_event_interruptible(Cblkp.txWq, Cblkp.Txq.NoOfChar < MAXSIZE);
    printk("After block\n");
    if(down_interruptible(&Cblkp.sem))
      return -ERESTARTSYS;
    avllen  = MAXSIZE - Cblkp.Txq.NoOfChar;
    copylen =  (culen < avllen) ? culen : avllen;
    if((Cblkp.Txq.WrIndex + copylen) <= MAXSIZE)
    {
      copy_from_user(Cblkp.Txq.Buf+Cblkp.Txq.WrIndex,buf,copylen);
    }
    else
    {
      partlen = MAXSIZE - Cblkp.Txq.WrIndex;
      copy_from_user(Cblkp.Txq.Buf+Cblkp.Txq.WrIndex,buf,partlen);
      copy_from_user(Cblkp.Txq.Buf,buf+partlen, copylen - partlen);
    }
    Cblkp.Txq.WrIndex =  (Cblkp.Txq.WrIndex + copylen) % MAXSIZE;
    Cblkp.Txq.NoOfChar  += copylen;

    culen = culen - copylen;
    outb(inb(Cblkp.baseAddr+IER) | 0x02, Cblkp.baseAddr+IER);/* enabled TI-IER => THR is empty? empty => TI  */
  }
  up(&Cblkp.sem);
  return count;
}


module_init(myserial_init);
module_exit(myserial_cleanup);
