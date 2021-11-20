ECHO_BIN = $(BUILDDIR)/echo.elf
ECHO_SRC = $(wildcard src/servers/*.cpp) $(wildcard src/lib/abi/*.cpp) \
	src/lib/tisix/log.cpp src/lib/tisix/fmt.cpp src/lib/tisix/string_view.cpp src/lib/tisix/scan.cpp src/lib/tisix/mem.cpp
ECHO_OBJ = \
	$(patsubst src/%.cpp, $(BUILDDIR)/%.cpp.o, $(ECHO_SRC)) \

TARGETS += $(ECHO_BIN)

$(BUILDDIR)/%.cpp.o: src/%.cpp
	$(DIR_GUARD)
	$(CROSS_CXX) -Isrc/lib/abi -c -o $@ $< $(CROSS_CXXFLAGS) -Os 

$(ECHO_BIN): $(ECHO_OBJ)
	$(DIR_GUARD)
	$(CROSS_LD) -o $@ $^ $(CROSS_ALDFLAGS)
