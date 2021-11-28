ECHO_BIN = $(BUILDDIR)/echo.elf
ECHO_SRC = $(wildcard src/servers/echo/*.cpp) $(wildcard src/lib/abi/*.cpp) $(wildcard src/lib/tisix/*.cpp)
ECHO_OBJ = \
	$(patsubst src/%.cpp, $(BUILDDIR)/%.cpp.o, $(ECHO_SRC)) \

TARGETS += $(ECHO_BIN)

$(BUILDDIR)/%.cpp.o: src/%.cpp
	$(DIR_GUARD)
	$(CROSS_CXX) -Isrc/lib/abi -c -o $@ $< $(CROSS_KCXXFLAGS)

$(ECHO_BIN): $(ECHO_OBJ)
	$(DIR_GUARD)
	$(CROSS_LD) -o $@ $^ $(CROSS_ALDFLAGS)
