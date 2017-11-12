
KDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

obj-m = procfs2.o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean