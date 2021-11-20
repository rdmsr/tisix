#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define PACKED [[gnu::packed]]
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define ALIGN_UP(__addr, __align) (((__addr) + (__align)-1) & ~((__align)-1))
#define ALIGN_DOWN(__addr, __align) ((__addr) & ~((__align)-1))

template <typename T>
static inline void volatile_write(uintptr_t address, T data)
{
    *((volatile T *)(address)) = data;
}

template <typename T>
static inline T volatile_read(uintptr_t address)
{
    return *((volatile T *)(address));
}

extern "C" void lock_acquire(uint32_t *lock);
extern "C" void lock_release(uint32_t *lock);