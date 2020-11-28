	obj-m := myioctl.o

	KERNELDIR= /lib/modules/$(shell uname -r)/build
	PWD=$(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KERNELFIR) M=$(PWD)
