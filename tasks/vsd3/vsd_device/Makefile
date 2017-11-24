MOD_NAME := vsd
subdir-ccflags-y := -Wall -Werror -Wextra -Wno-unused

ifneq ($(KERNELRELEASE),)
	obj-m := $(MOD_NAME).o
	header-y += vsd_hw.h
	$(MOD_NAME)-objs := module.o
else
	include ../sub_config.mk
all:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp $(MOD_NAME).ko $(VM_SHARE_PATH)

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
endif
