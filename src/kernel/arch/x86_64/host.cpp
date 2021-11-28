#include "tisix/host.hpp"
#include "loader.hpp"
#include "tisix/maybe.hpp"
#include <devices/com.hpp>
#include <pmm.hpp>
#include <vmm.hpp>

using namespace tisix;

void host_log_putc(char c)
{
    com_putc(COM1, c);
}

void host_log_write(const char *s)
{
    com_write(COM1, s);
}

const char *host_get_current_task()
{
    return nullptr;
}

void *host_allocate_pages(size_t pages)
{
    return pmm_allocate_zero(pages).value_or(panic_to_lambda("Allocation failed"));
}

void host_free_pages(void *addr, size_t pages)
{
    pmm_free(addr, pages);
}

void host_map_memory(uintptr_t *pagemap, uint64_t phys_addr, uint64_t virt_addr, uint64_t flags)
{
    vmm_map_page(pagemap, phys_addr, virt_addr, flags);
}