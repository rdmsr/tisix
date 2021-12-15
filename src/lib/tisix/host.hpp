#pragma once
#include <tisix/maybe.hpp>
#include <tisix/std.hpp>

extern "C"
{
    void host_log_write(const char *s);
    void host_log_putc(char c);

    void *host_allocate_pages(size_t pages);
    void host_free_pages(void *addr, size_t pages);

    const char *host_get_current_task();
    void host_map_memory(uintptr_t *pagemap, uint64_t phys_addr, uint64_t virt_addr, uint64_t flags);

    void host_alloc_init();

    void *host_get_alloc(size_t size = 256);
}
