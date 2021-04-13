BUILD_DIR := /lib/modules/$(shell uname -r)/build
CFILES := test1.c test2.c
CUR_DIR := $(shell pwd)
MAKE := make -C $(BUILD_DIR) M=$(CUR_DIR)

ccflags-y += -Ofast -Wall -Werror -march=native -std=gnu2x
obj-m := test.o
test-objs := $(CFILES:%.c=%.o)

all:
	$(MAKE) modules

clean:
	$(MAKE) clean

.PHONY: all clean
