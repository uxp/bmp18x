CONFIG_BMP18X_I2C := y
CONFIG_BMP18X_SPI := y

obj-m := bmp18x-core.o
ifeq ($(CONFIG_BMP18X_I2C),y)
obj-m += bmp18x-i2c.o
endif
ifeq ($(CONFIG_BMP18X_SPI),y)
obj-m += bmp18x-spi.o
endif

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -f Module.symvers modules.order *.o *.mod.* *.ko
