obj-m = dr.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	rm -rf *.o *.ko *.mod.c *.mod *.order *.symvers

