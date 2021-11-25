INIT_BIN = $(BUILDDIR)/init.elf
INIT_SRC = $(wildcard src/servers/init/*.cpp) $(wildcard src/lib/abi/*.cpp) \
	src/lib/tisix/log.cpp src/lib/tisix/fmt.cpp src/lib/tisix/string_view.cpp src/lib/tisix/scan.cpp src/lib/tisix/mem.cpp
INIT_OBJ = \
	$(patsubst src/%.cpp, $(BUILDDIR)/%.cpp.o, $(INIT_SRC)) \

TARGETS += $(INIT_BIN)

$(BUILDDIR)/%.cpp.o: src/%.cpp
	$(DIR_GUARD)
	$(CROSS_CXX) -Isrc/lib/abi -c -o $@ $< $(CROSS_CXXFLAGS) -Os 

$(INIT_BIN): $(INIT_OBJ)
	$(DIR_GUARD)
	$(CROSS_LD) -o $@ $^ $(CROSS_ALDFLAGS)
