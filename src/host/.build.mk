BUILDDIR_HOST= $(BUILDDIR)/host

HOST = $(BUILDDIR_HOST)/host.elf

HOST_SRC = $(shell find src/host -name '*.cpp') \
		  $(shell find src/lib/tisix -name '*.cpp') 

HOST_OBJ = $(patsubst src/%, $(BUILDDIR_HOST)/%.o, $(HOST_SRC))

$(BUILDDIR_HOST)/%.cpp.o: src/%.cpp
	$(ECHO) "\e[37mhost \e[92mCXX\e[m" $<
	@$(DIR_GUARD)
	@$(CROSS_CXX) -c -o $@ $< $(CROSS_CXXFLAGS)

$(HOST): $(HOST_OBJ)
	$(ECHO) "\e[37mhost \e[92mLD\e[m" $@
	@$(DIR_GUARD)
	@$(CROSS_CXX) -o $@ $^

all: $(HOST)