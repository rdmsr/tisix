#pragma once

#include <tisix/stream.hpp>

typedef enum
{
    COM1 = 0x3F8,
    COM2 = 0x2F8,
    COM3 = 0x3E8,
    COM4 = 0x2E8
} ComPort;

class COM : public tisix::Stream<const char *>
{
public:
    void write(const char *s) override;
    const char *read() override;
    COM(ComPort p);
    void putc(char c) override;

private:
    ComPort port;
};