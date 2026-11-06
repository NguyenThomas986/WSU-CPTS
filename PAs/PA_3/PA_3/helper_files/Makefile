APP_EXTRA_FLAGS := -O2 -std=c11 -Wall -Wextra -Wpedantic
KERNEL_SRC := /lib/modules/$(shell uname -r)/build
SUBDIR := $(CURDIR)
GCC := gcc

.PHONY: all clean modules app

all: modules app

obj-m := kmlab.o

modules:
	$(MAKE) -C $(KERNEL_SRC) M=$(SUBDIR) modules

app: userapp

userapp: userapp.c userapp.h
	$(GCC) $(APP_EXTRA_FLAGS) -o $@ userapp.c

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(SUBDIR) clean
	$(RM) userapp
