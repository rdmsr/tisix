#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/log.hpp>
#include <tisix/std.hpp>

struct PACKED Pixel
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t __unused;
};

extern "C" void _start(tisix::Handover *handover)
{

    log("hello from the fb server");

    log("Drawing to the screen....");

    auto fb = handover->framebuffer;

    auto framebuffer = (Pixel *)handover->framebuffer.addr;

    for (size_t x = 0; x < fb.width; x++)
    {
        for (size_t y = 0; y < fb.height; y++)
        {
            size_t raw_position = x + y * fb.width;

            framebuffer[raw_position].blue = x ^ y;
            framebuffer[raw_position].red = (y * 2) ^ (x * 2);
            framebuffer[raw_position].green = (y * 4) ^ (x * 4);
        }
    }

    while (1)
        ;
}