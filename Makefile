BUILD_DIR := /lib/modules/$(shell uname -r)/build
CFILES := study-exit.c study-init.c study-main.c
CUR_DIR := $(shell pwd)
MAKE := make -C $(BUILD_DIR) M=$(CUR_DIR)

ccflags-y += -Ofast -Wall -Werror -Wno-declaration-after-statement
ccflags-y += -march=native -std=gnu2x
obj-m := study.o
study-objs := $(CFILES:%.c=%.o)

all:
	+$(MAKE) modules

clean:
	$(MAKE) clean

.PHONY: all clean
