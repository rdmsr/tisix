# Build system from brutal.


.DEFAULT_GOAL := all
SHELL := /bin/bash

export LC_ALL=C

ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
      -nrRf $(firstword $(MAKEFILE_LIST)) \
      ECHO="%COUNT_ME%" | grep -c "%COUNT_ME%")

N := x
C = $(words $N)$(eval N := x $N)

ECHO = @echo -e "`printf "\e[1m[$C/$T]\e[m" `"
endif

CXXFLAGS_STD ?= \
		  -std=c++17 -g

CXXFLAGS_OPT ?= \
		  -O2

CXXFLAGS_WARNS ?= \
		  -Wall \
		  -Werror \
		  -Wextra 

CXXFLAGS_INC ?= \
				-Isrc/lib 


ifneq ($(CONFIG_ARCH), 0)
	BUILDDIR = build/$(CONFIG_ARCH)-$(CONFIG_TOOLCHAIN)
else
	BUILDDIR = build/host-$(CONFIG_TOOLCHAIN)
endif

DIR_GUARD = mkdir -p $(@D)

include meta/default.mk
include meta/toolchain/$(CONFIG_TOOLCHAIN)/.build.mk


ifneq ($(CONFIG_ARCH), 0)
	include meta/arch/$(CONFIG_ARCH).mk
	include src/kernel/.build.mk
else
	include meta/arch/host.mk
	include src/host/.build.mk
endif

include src/servers/.build.mk

SERVERS += $(ECHO_BIN) $(FB_BIN) $(INIT_BIN)

include sysroot/.build.mk

.PHONY = all

TARGETS += $(ISO)

all: $(TARGETS)

tests: $(HOST)
	$^

run: $(TARGETS)
	qemu-system-$(CONFIG_ARCH) $(QEMU_ARGS) -serial stdio -no-reboot -no-shutdown -cdrom $(ISO)

debug: $(TARGETS)
	qemu-system-$(CONFIG_ARCH) -d int -no-reboot -no-shutdown -m 256M -serial stdio -cdrom $(ISO)


.PHONY: clean
clean:
	rm -rf build
