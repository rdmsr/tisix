#include "tisix/host.hpp"
#include "abi/syscalls.hpp"
#include "tisix/arch.hpp"
#include "tisix/std.hpp"
#include "tisix/string_view.hpp"
#include <syscalls.hpp>

void host_log_write(const char *s)
{
    tx_sys_debug(s);
}

void arch_panic_impl(const char *file, int line, tisix::StringView fmt, tisix::FmtArgs args)
{
    (void)file;
    (void)line;
    (void)fmt;
    (void)args;
}

void lock_acquire(uint32_t *lock) { (void)lock; }
void lock_release(uint32_t *lock) { (void)lock; }