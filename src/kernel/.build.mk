BUILDDIR_KERNEL = $(BUILDDIR)/kernel

KERNEL = $(BUILDDIR_KERNEL)/kernel.elf

KERNEL_SRC = $(shell find src/kernel/arch/$(CONFIG_ARCH) -name '*.cpp') \
			 $(wildcard src/kernel/*.cpp) \
			 $(shell find src/kernel/arch/$(CONFIG_ARCH) -name '*.asm') \
			 $(shell find src/lib/tisix -name '*.cpp')

KERNEL_OBJ = $(patsubst src/%, $(BUILDDIR_KERNEL)/%.o, $(KERNEL_SRC))

$(BUILDDIR_KERNEL)/%.cpp.o: src/%.cpp
	$(ECHO) "\e[37mkernel \e[92mCXX\e[m" $<
	@$(DIR_GUARD)
	@$(CROSS_CXX) -c -o $@ $< $(CROSS_KCXXFLAGS)

$(BUILDDIR_KERNEL)/%.asm.o: src/%.asm
	$(ECHO) "\e[37mkernel \e[92mAS\e[m" $<
	@$(DIR_GUARD)
	@$(CROSS_AS) $(CROSS_ASFLAGS) $< -o $@

$(KERNEL): $(KERNEL_OBJ)
	$(ECHO) "\e[37mkernel \e[92mLD\e[m" $@
	@$(DIR_GUARD)
	@$(CROSS_LD) -o $@ $^ $(CROSS_KLDFLAGS)
