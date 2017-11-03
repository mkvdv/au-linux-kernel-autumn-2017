INCLUDE_PATH=../vsd_driver/
include ../sub_config.mk
CFLAGS+=-Wall -Werror -Wextra -pedantic

all: vsdio vsd_test

vsdio: vsdio.c vsd_device.c vsd_device.h
	gcc $(CFLAGS) $(wordlist 1, 2, $^) -I$(INCLUDE_PATH) -o $@
	cp $@ $(VM_SHARE_PATH)

vsd_test: vsd_test.c vsd_device.c vsd_device.h
	gcc $(CFLAGS) $(wordlist 1, 2, $^) -I$(INCLUDE_PATH) -o $@
	cp $@ $(VM_SHARE_PATH)

.PHONY: clean

clean:
	rm -rf vsdio
	rm -rf vsd_test
