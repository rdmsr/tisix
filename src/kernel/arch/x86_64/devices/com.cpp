#include "com.hpp"
#include <asm.hpp>

using namespace tisix;

static int is_transmit_empty(ComPort port)
{
    return asm_in8(port + 5) & 0x20;
}

void tisix::com_initialize(ComPort port)
{
    asm_out8(port + 1, 0x00);
    asm_out8(port + 3, 0x80);
    asm_out8(port + 0, 0x03);
    asm_out8(port + 1, 0x00);
    asm_out8(port + 3, 0x03);
    asm_out8(port + 2, 0xC7);
    asm_out8(port + 4, 0x0B);
}

void tisix::com_write(ComPort port, const char *str)
{
    while (*str)
    {
        com_putc(port, *str++);
    }
}

void tisix::com_putc(ComPort port, char c)
{
    while (is_transmit_empty(port) == 0)
        ;
    asm_out8(port, c);
}

const char *com_read()
{
    return "not implemented";
}