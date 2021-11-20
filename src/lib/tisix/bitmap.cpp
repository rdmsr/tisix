#include "tisix/arch.hpp"
#include <tisix/bitmap.hpp>

using namespace tisix;

Bitmap::Bitmap()
{
    this->buffer = nullptr;
    this->size = 0;
}

Bitmap::Bitmap(uint8_t *buf, size_t length)
{
    this->buffer = buf;
    this->size = length;
}

Bitmap::Bitmap(uintptr_t address, size_t size)
{
    this->buffer = (uint8_t *)(address);
    this->size = size;
}

bool Bitmap::operator[](size_t index)
{
    uint64_t byte = index / 8;
    uint64_t bit = index % 8;

    return this->buffer[byte] & (1 << bit);
}

void Bitmap::set(size_t index)
{
    uint64_t bit = index % 8;
    uint64_t byte = index / 8;

    this->buffer[byte] |= (1 << bit);
}

void Bitmap::clear(size_t index)
{
    uint64_t bit = index % 8;
    uint64_t byte = index / 8;

    this->buffer[byte] &= ~(1 << bit);
}

uint8_t *Bitmap::begin()
{
    return this->buffer;
}

uint8_t *Bitmap::end()
{
    return this->buffer + this->size;
}

void Bitmap::fill(uint8_t value)
{
    memset(this->buffer, value, this->size);
}