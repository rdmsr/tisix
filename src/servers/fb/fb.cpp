#include "pmm.hpp"
#include "tisix/fmt.hpp"
#include "tisix/gfx/canvas.hpp"
#include "tisix/handover.hpp"
#include <abi/layer.hpp>
#include <abi/syscalls.hpp>
#include <tisix/gfx/canvas.hpp>
#include <tisix/log.hpp>
#include <tisix/std.hpp>

using namespace tisix;

int handover_main(Handover *handover)
{
    log("Framebuffer is at {#p}", (void *)handover->framebuffer.addr);

    auto fb = handover->framebuffer;

    log("Resolution is {}x{}", fb.width, fb.height);

    gfx::Canvas canvas(fb.addr, fb.width, fb.height);

    log("Drawing to the screen...");

    for (size_t j = 0; j < fb.height; j++)
    {
        canvas.draw_pixel(canvas.color(0xFFFFFF), 0, j);
    }

    return 0;
}
