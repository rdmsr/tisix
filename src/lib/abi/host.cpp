#include <abi/syscalls.hpp>
#include <syscalls.hpp>
#include <tisix/alloc.hpp>
#include <tisix/arch.hpp>
#include <tisix/host.hpp>
#include <tisix/std.hpp>
#include <tisix/string_view.hpp>

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

extern "C" void *host_get_alloc(size_t size)
{
    (void)size;
    return nullptr;
}

void host_free_pages(void *addr, size_t pages)
{
    tx_sys_free(addr, pages);
}
} // namespace tisix
  // namespace tisix