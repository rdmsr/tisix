#include "asm.hpp"
#include "tisix/host.hpp"
#include "tisix/maybe.hpp"
#include "tisix/std.hpp"
#include <devices/com.hpp>
#include <pmm.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>

using namespace tisix;

void arch_hang()
{
    while (1)
    {
        asm_cli();
        asm_hlt();
    }

    assert_unreachable();
}

const char *get_arch_name()
{
    return "x86_64";
}

namespace tisix
{

void *liballoc_alloc(int n)
{
    return (void *)((uintptr_t)host_allocate_pages(n) + MMAP_IO_BASE);
}

void liballoc_free(void *ptr, int n)
{
    host_free_pages(ptr, n);
}

void liballoc_lock() {}
void liballoc_unlock() {}

} // namespace tisix

void arch_panic_impl(const char *file, int line, tisix::StringView fmt, tisix::FmtArgs args)
{
    fmt_stream(host_log_write, "\n------- \033[1;31mKernel panic\033[0m at {}:{} -------\n", file, line);

    fmt_stream_impl(host_log_write, fmt, args);

    fmt_stream(host_log_write, "\n");

    arch_hang();
}

uint32_t arch_in(uint16_t port, int size)
{
    if (size == 1)
    {
        return asm_in8(port);
    }
    else if (size == 2)
    {
        return asm_in16(port);
    }
    else if (size == 4)
    {
        return asm_in32(port);
    }
    else
    {
        return 0;
    }
}

void arch_out(uint16_t port, uint32_t data, int size)
{
    if (size == 1)
    {
        asm_out8(port, data);
    }
    else if (size == 2)
    {
        asm_out16(port, data);
    }
    else if (size == 4)
    {
        asm_out32(port, data);
    }
}