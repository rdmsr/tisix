#pragma once
#include <tisix/std.hpp>

namespace tisix::gfx
{

struct RawPixel
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t __unused;
};

struct PACKED Color
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class Canvas
{
public:
    Canvas(RawPixel *buf, int width, int height);
    Canvas(uint64_t addr, int width, int height)
    {
        this->buffer = (RawPixel *)addr;
        this->width = width;
        this->height = height;
    }

    void draw_pixel(Color color, int x, int y);

    Color color(uint8_t r, uint8_t g, uint8_t b)
    {
        return {r, g, b};
    }

    Color color(uint32_t hex)
    {
        uint8_t r = hex >> 16;
        uint8_t g = hex >> 8;
        uint8_t b = hex;

        return {r, g, b};
    }

    int width = 0, height = 0;

private:
    RawPixel *buffer;
};

} // namespace tisix::gfx
