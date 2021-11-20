#pragma once

#include <tisix/stream.hpp>

namespace tisix
{
typedef enum
{
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8
} ComPort;

void com_initialize(ComPort port);
void com_write(ComPort port, const char *string);
void com_putc(ComPort port, char c);
} // namespace tisix