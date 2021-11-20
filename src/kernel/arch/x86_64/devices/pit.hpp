#pragma once
#include <asm.hpp>

inline uint32_t pit_read_count(void)
{
    asm_out8(0x43, 0);
    uint32_t counter = asm_in8(0x40);
    counter |= asm_in8(0x40) << 8;

    return counter;
}

inline void pit_initialize(int hz)
{
    int divisor = 1193180 / hz;

    asm_out8(0x43, 0x36);
    asm_out8(0x40, divisor & 0xff);
    asm_out8(0x40, (divisor >> 8) & 0xFF);
}

inline void pit_usleep(uint16_t ms)
{
    asm_out8(0x43, 0x30);
    asm_out8(0x40, ms & 0xff);
    asm_out8(0x40, (ms >> 8) & 0xff);

    while (pit_read_count() == 0)
        ;
}
