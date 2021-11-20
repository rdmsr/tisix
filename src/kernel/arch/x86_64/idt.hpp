#pragma once
#include <tisix/log.hpp>

#define INTGATE 0x8e
#define TRAPGATE 0xef

struct PACKED IdtPointer
{
    uint16_t size;
    uint64_t addr;
};

struct PACKED IdtDescriptor
{
    uint16_t offset_lo;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_hi;
    uint32_t zero;
};

void idt_initialize(void);