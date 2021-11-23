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

    auto fb = handover->framebuffer;

    log("Resolution is {}x{}", fb.width, fb.height);

    log("Drawing to the screen....");

    auto framebuffer = (Pixel *)fb.addr;

    TxEvent event = {.type = TX_EVENT_IRQ, .irq = 0};

    tx_sys_bind(&event);

    TxIpc ipc;

    ipc.flags = TX_IPC_RECV;

    int current_offset = 0;

    while (tx_sys_ipc(&ipc) == TX_SUCCESS)
    {
        if (ipc.msg.event.irq == 0)
        {

            for (size_t x = 0; x < fb.width; x++)
            {
                for (size_t y = 0; y < fb.height; y++)
                {
                    size_t raw_position = x + y * fb.width;
                    framebuffer[raw_position].blue = (x + current_offset) ^ (y + current_offset);
                    framebuffer[raw_position].red = (y * 2 + current_offset) ^ (x * 2 + current_offset);
                    framebuffer[raw_position].green = (y * 4 + current_offset) ^ (x * 4 + current_offset);
                }
            }

            current_offset++;
        }
    }

    tx_sys_exit(TX_SUCCESS);
}