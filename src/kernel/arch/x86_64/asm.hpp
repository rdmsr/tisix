#pragma once

#include <tisix/std.hpp>

static inline void asm_cli(void) { asm volatile("cli"); }

static inline void asm_sti(void) { asm volatile("sti"); }

static inline void asm_hlt(void) { asm volatile("hlt"); }

static inline void asm_pause(void) { asm volatile("pause"); }

static inline uint8_t asm_in8(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline uint16_t asm_in16(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline uint32_t asm_in32(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %1, %0"
                 : "=a"(data)
                 : "d"(port));
    return data;
}

static inline void asm_out8(uint16_t port, uint8_t data)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(data), "d"(port));
}

static inline void asm_out16(uint16_t port, uint16_t data)
{
    asm volatile("outw %0, %1"
                 :
                 : "a"(data), "d"(port));
}

static inline void asm_out32(uint16_t port, uint32_t data)
{
    asm volatile("outl %0, %1"
                 :
                 : "a"(data), "d"(port));
}

#define ASM_MAKE_CRN(N)                                \
    static inline uint64_t asm_read_cr##N(void)        \
    {                                                  \
        uint64_t value = 0;                            \
        asm volatile("mov %%cr" #N ", %0"              \
                     : "=r"(value));                   \
        return value;                                  \
    }                                                  \
                                                       \
    static inline void asm_write_cr##N(uint64_t value) \
    {                                                  \
        asm volatile("mov %0, %%cr" #N ::"a"(value));  \
    }

ASM_MAKE_CRN(0);
ASM_MAKE_CRN(1);
ASM_MAKE_CRN(2);
ASM_MAKE_CRN(3);