TIME_BIN = $(BUILDDIR)/time.elf
TIME_SRC = $(wildcard src/servers/time/*.cpp) $(wildcard src/lib/abi/*.cpp) $(wildcard src/lib/tisix/*.cpp)
TIME_OBJ = \
	$(patsubst src/%.cpp, $(BUILDDIR)/%.cpp.o, $(TIME_SRC)) \

TARGETS += $(TIME_BIN)

$(BUILDDIR)/%.cpp.o: src/%.cpp
	$(DIR_GUARD)
	$(CROSS_CXX) -Isrc/lib/abi -c -o $@ $< $(CROSS_CXXFLAGS) -Os 

$(TIME_BIN): $(TIME_OBJ)
	$(DIR_GUARD)
	$(CROSS_LD) -o $@ $^ $(CROSS_ALDFLAGS)
