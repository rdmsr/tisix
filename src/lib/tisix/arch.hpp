#pragma once

#include <tisix/fmt.hpp>
#include <tisix/handover.hpp>
#include <tisix/maybe.hpp>
#include <tisix/std.hpp>
#include <tisix/stream.hpp>
#include <tisix/string_view.hpp>

#define KERNEL_STACK_SIZE (16384)
#define USER_STACK_BASE (0xc0000000)

class MemoryAllocator
{
public:
    virtual tisix::Maybe<void *> allocate(size_t pages)
    {
        (void)pages;
        return Nothing;
    };

    virtual tisix::Maybe<void *> allocate_zero(size_t pages)
    {
        (void)pages;
        return Nothing;
    };

    virtual void free(void *addr, size_t pages)
    {
        (void)addr;
        (void)pages;
    };
};

struct Arch
{
    tisix::Stream<const char *> *debug_stream;
    const char *name;
    MemoryAllocator *allocator;
    uint64_t *kernel_pagemap;
};

Arch *get_arch();

extern "C" void arch_entry_main(tisix::Handover *handover);

void arch_hang();

void arch_panic_impl(const char *file, int line, tisix::StringView fmt, tisix::FmtArgs args);

#define panic(fmt, ...) arch_panic_impl(__FILENAME__, __LINE__, fmt, tisix::fmt_make_storage(__VA_ARGS__).args);