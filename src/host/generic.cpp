#include "tisix/alloc.hpp"
#include "tisix/host.hpp"
#include "tisix/log.hpp"
#include "tisix/std.hpp"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <tisix/arch.hpp>
#include <tisix/test.hpp>

/* Arch stuff ----------- */
using tisix::Maybe;

void lock_acquire(uint32_t *lock) { (void)lock; };
void lock_release(uint32_t *lock) { (void)lock; };

void host_log_write(const char *s)
{
    printf("%s", s);
}

void host_log_putc(char c)
{
    printf("%c", c);
}

void arch_panic_impl(const char *file, int line, tisix::StringView fmt, tisix::FmtArgs args)
{
    log_error("Panic at {}:{}", file, line);

    tisix::log_impl(file, line, fmt, args);
}

int main()
{

    return tisix::test_run_all();
}