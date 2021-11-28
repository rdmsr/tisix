#include "tisix/host.hpp"
#include "abi/syscalls.hpp"
#include "pmm.hpp"
#include "tisix/alloc.hpp"
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

namespace tisix
{

void *host_allocate_pages(size_t pages)
{
    uint64_t _ptr;
    uint64_t *ptr = &_ptr;

    tx_sys_alloc(pages, (void **)&ptr);

    return (void *)ptr;
}

void host_free_pages(void *addr, size_t pages)
{
    tx_sys_free(addr, pages);
}

void *liballoc_alloc(int pages)
{
    return (void *)((uint64_t)host_allocate_pages(pages) + MMAP_IO_BASE);
}

int liballoc_free(void *p, int s)
{
    host_free_pages(p, s);
    return 0;
};

int liballoc_lock() { return 0; }
int liballoc_unlock() { return 0; }
} // namespace tisix