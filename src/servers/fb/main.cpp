#include "tisix/gfx/canvas.hpp"
#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/gfx/canvas.hpp>
#include <tisix/log.hpp>
#include <tisix/std.hpp>

using namespace tisix;

extern "C" void _start(Handover *handover)
{

    log("hello from the fb server");

    auto fb = handover->framebuffer;

    log("Resolution is {}x{}", fb.width, fb.height);

    gfx::Canvas canvas(fb.addr, fb.width, fb.height);

    for (size_t i = 0; i < fb.width; i++)
    {
        for (size_t j = 0; j < fb.height; j++)
        {
            canvas.draw_pixel(canvas.color(40, 44, 52), i, j);
        }
    }

    tx_sys_exit(TX_SUCCESS);
}