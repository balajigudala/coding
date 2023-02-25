obj-m += ioctl_module.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build  M=$(shell pwd) modules
 
clean:
	make -C $(KDIR)  M=$(shell pwd) clean
