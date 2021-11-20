ifneq ($(CONFIG_ARCH), 0)
CROSS_CXX=clang++ -target $(CONFIG_ARCH)-none-elf
CROSS_CXXFLAGS= \
	$(CXXFLAGS_STD) \
	$(CXXFLAGS_OPT) \
	$(CXXFLAGS_WARNS) \
	$(ARCH_CXXFLAGS) \
	$(CXXFLAGS_INC) \
	-ffreestanding \
	-fno-rtti \
	-fno-exceptions

CROSS_KCXXFLAGS= \
	$(CROSS_CXXFLAGS) \
	$(ARCH_KCXXFLAGS) \
	-Isrc/kernel \
	-fno-stack-protector \
	-D__kernel__=1

CROSS_UCXXFLAGS= \
	$(CROSS_CXXFLAGS) \
	-nostdlib

CROSS_LD=ld.lld
CROSS_KLDFLAGS= \
	-Tsrc/kernel/arch/$(CONFIG_ARCH)/link.ld \
	-z max-page-size=0x1000 \
	$(ARCH_LDFLAGS)

CROSS_ULDFLAGS= \
	-Tsrc/kernel/arch/$(CONFIG_ARCH)/link.ld \
	-z max-page-size=0x1000 \
	$(ARCH_LDFLAGS)

CROSS_AR=$(CONFIG_ARCH)-elf-ar
CROSS_ARFLAGS=rcs

else
	CROSS_CXX=clang++
	CROSS_CXXFLAGS = $(ARCH_CXXFLAGS)

	CROSS_LD=ld.lld
endif

