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

private:
    RawPixel *buffer;
    int width = 0, height = 0;
};

} // namespace tisix::gfx