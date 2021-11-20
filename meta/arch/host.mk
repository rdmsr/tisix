ARCH_CXXFLAGS ?= \
	-fno-rtti \
	-fno-builtin \
	-fno-stack-protector \
	$(CXXFLAGS_INC) \
	$(CXXFLAGS_STD) \
	$(CXXFLAGS_WARNS) \
	-DHOST \
	-ggdb