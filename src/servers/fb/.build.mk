FB_BIN = $(BUILDDIR)/fb.elf
FB_SRC = $(wildcard src/servers/fb/*.cpp) $(wildcard src/lib/abi/*.cpp) $(wildcard src/lib/tisix/*.cpp) $(wildcard src/lib/tisix/*/*.cpp)
FB_OBJ = \
	$(patsubst src/%.cpp, $(BUILDDIR)/%.cpp.o, $(FB_SRC)) \

TARGETS += $(FB_BIN)

$(BUILDDIR)/%.cpp.o: src/%.cpp
	$(DIR_GUARD)
	$(CROSS_CXX) -Isrc/lib/abi -c -o $@ $< $(CROSS_CXXFLAGS) -Os 

$(FB_BIN): $(FB_OBJ)
	$(DIR_GUARD)
	$(CROSS_LD) -o $@ $^ $(CROSS_ALDFLAGS)

