#include "asm.hpp"
#include "tisix/maybe.hpp"
#include "tisix/std.hpp"
#include <devices/com.hpp>
#include <pmm.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>

using namespace tisix;

static Arch arch = (Arch){.debug_stream = nullptr, .name = "x86_64", .allocator = nullptr, .kernel_pagemap = nullptr};

Arch *get_arch()
{
    return &arch;
}

void arch_hang()
{
    while (1)
    {
        asm_cli();
        asm_hlt();
    }

    assert_unreachable();
}

namespace tisix
{
void *liballoc_alloc(int n)
{
    return (void *)((uintptr_t)arch.allocator->allocate(n).value_or(panic_to_lambda("Failed allocating memory")) + MMAP_IO_BASE);
}

void liballoc_free(void *ptr, int n)
{
    arch.allocator->free(ptr, n);
}

void liballoc_lock() {}
void liballoc_unlock() {}

} // namespace tisix
void arch_panic_impl(const char *file, int line, tisix::StringView fmt, tisix::FmtArgs args)
{
    fmt_stream(get_arch()->debug_stream, "\n------- \033[1;31mKernel panic\033[0m at {}:{} -------\n", file, line);

    fmt_stream_impl(get_arch()->debug_stream, fmt, args);

    fmt_stream(get_arch()->debug_stream, "\n");

    arch_hang();
}
