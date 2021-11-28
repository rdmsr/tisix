#pragma once

#include <tisix/fmt.hpp>
#include <tisix/handover.hpp>
#include <tisix/host.hpp>
#include <tisix/maybe.hpp>
#include <tisix/std.hpp>
#include <tisix/stream.hpp>
#include <tisix/string_view.hpp>

#define KERNEL_STACK_SIZE (16384)
#define USER_STACK_BASE (0xc0000000)

extern "C"
{
    const char *get_arch_name();
    void arch_entry_main(tisix::Handover *handover);
    void arch_hang();
    void arch_panic_impl(const char *file, int line, tisix::StringView fmt, tisix::FmtArgs args);

    uint32_t arch_in(uint16_t port, int size);
    void arch_out(uint16_t port, uint32_t data, int size);
}

#define panic(fmt, ...) arch_panic_impl(__FILENAME__, __LINE__, fmt, tisix::fmt_make_storage(__VA_ARGS__).args);