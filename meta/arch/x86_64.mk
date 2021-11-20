ARCH_KCXXFLAGS = \
	-fno-stack-protector \
	-mno-80387 \
	-mno-mmx \
	-mno-3dnow \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel -fno-use-cxa-atexit

ARCH_CXXFLAGS = \
	-Isrc/kernel/arch/x86_64 \
	-fsanitize=undefined 

CROSS_AS=nasm
CROSS_ASFLAGS=-g -felf64

QEMU_ARGS= \
		-m 256M \
		-enable-kvm \
		-rtc base=localtime 

