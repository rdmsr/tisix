#pragma once
#include <tisix/mem.hpp>

namespace tisix
{
class Bitmap
{
private:
    uint8_t *buffer;

public:
    size_t size;

public:
    Bitmap(uintptr_t address, size_t length);
    Bitmap(uint8_t *buf, size_t length);
    Bitmap();

    void fill(uint8_t value);
    void set(size_t index);
    void clear(size_t index);

    bool operator[](size_t index);

    uint8_t *begin();
    uint8_t *end();
};

} // namespace tisix