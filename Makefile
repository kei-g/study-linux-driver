BUILD_DIR := /lib/modules/$(shell uname -r)/build
CFILES := study-exit.c study-init.c study-main.c
CUR_DIR := $(shell pwd)
MAKE := make -C $(BUILD_DIR) M=$(CUR_DIR)
RULES_DIR = /etc/udev/rules.d
STRIP := strip --strip-debug --strip-unneeded
TARGET_BINARY := study.ko

ccflags-y += -Ofast -Wall -Werror -Wno-declaration-after-statement
ccflags-y += -march=native -std=gnu2x
obj-m := study.o
study-objs := $(CFILES:%.c=%.o)

all: $(TARGET_BINARY)

clean:
	$(MAKE) clean

disasm: $(TARGET_BINARY)
	llvm-objdump --disassemble-all $(TARGET_BINARY) | less

nm: $(TARGET_BINARY)
	nm -gP $(TARGET_BINARY) | less

readelf: $(TARGET_BINARY)
	readelf --all $(TARGET_BINARY) | less

rules:
	echo 'KERNEL=="study[0-9]*", GROUP="root", MODE="0666"' | sudo tee $(RULES_DIR)/21-study.rules > /dev/null

test: $(TARGET_BINARY)
	sudo insmod $(TARGET_BINARY)
	sudo cat /dev/study0
	echo -n say hello > /dev/study0
	sudo rmmod $(TARGET_BINARY)

.PHONY: all clean disasm nm readelf rules test

$(TARGET_BINARY):
	+$(MAKE) modules && $(STRIP) $@
