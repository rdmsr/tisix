#include "com.hpp"
#include <asm.hpp>

static int is_transmit_empty(ComPort port)
{
    return asm_in8(port + 5) & 0x20;
}

COM::COM(ComPort port)
{
    this->port = port;

    asm_out8(port + 1, 0x00);
    asm_out8(port + 3, 0x80);
    asm_out8(port + 0, 0x03);
    asm_out8(port + 1, 0x00);
    asm_out8(port + 3, 0x03);
    asm_out8(port + 2, 0xC7);
    asm_out8(port + 4, 0x0B);
}

void COM::write(const char *str)
{
    while (*str)
    {
        putc(*str++);
    }
}

void COM::putc(char c)
{
    while (is_transmit_empty(port) == 0)
        ;
    asm_out8(port, c);
}

const char *COM::read()
{
    return "not implemented";
}