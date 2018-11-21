obj-m := gpio_device_driver.o
KDIR := /home/u201402324/rpi/linux

default:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
