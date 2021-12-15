ifneq ($(CONFIG_ARCH), 0)
	CROSS_CXX=$(CONFIG_ARCH)-elf-g++
	CROSS_LD=$(CONFIG_ARCH)-elf-ld

	CROSS_CXXFLAGS= \
		$(CXXFLAGS_STD) \
		$(CXXFLAGS_OPT) \
		$(CXXFLAGS_WARNS) \
		$(ARCH_CXXFLAGS) \
		$(CXXFLAGS_INC) \
		-ffreestanding \
		-fno-rtti \
		-fno-exceptions \
		-Wno-array-bounds 

	CROSS_KCXXFLAGS= \
		$(CROSS_CXXFLAGS) \
		$(ARCH_KCXXFLAGS) \
		-Isrc/kernel \
		-fno-stack-protector \
		-D__kernel__=1

	CROSS_UCXXFLAGS= \
		$(CROSS_CXXFLAGS) \
		-nostdlib

	CROSS_KLDFLAGS= \
		-Tsrc/kernel/arch/$(CONFIG_ARCH)/link.ld \
		-z max-page-size=0x1000 \
		$(ARCH_LDFLAGS)

	CROSS_ULDFLAGS= \
		-Tsrc/kernel/arch/$(CONFIG_ARCH)/link.ld \
		-z max-page-size=0x1000 \
		$(ARCH_LDFLAGS)

	CROSS_ALDFLAGS = \
		-Tsrc/kernel/arch/$(CONFIG_ARCH)/app_link.ld \
		-z max-page-size=0x1000 \
		$(ARCH_LDFLAGS)

	CROSS_AR=$(CONFIG_ARCH)-elf-ar
	CROSS_ARFLAGS=rcs

else
	CROSS_CXX=g++
	CROSS_LD=ld
	CROSS_CXXFLAGS = $(ARCH_CXXFLAGS)
endif

