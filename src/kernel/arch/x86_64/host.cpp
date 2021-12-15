#include "tisix/host.hpp"
#include "loader.hpp"
#include "tisix/arch.hpp"
#include "tisix/maybe.hpp"
#include <devices/com.hpp>
#include <pmm.hpp>
#include <vmm.hpp>

using namespace tisix;

static Alloc alloc = {};
static Alloc page_alloc = {};
static Alloc stack_alloc = {};

void host_alloc_init()
{
    void *buf = (void *)((uint64_t)host_allocate_pages(2) + MMAP_IO_BASE);

    alloc.construct((uint8_t *)buf, 8192, 256);

    void *page_buf = (void *)((uint64_t)host_allocate_pages(4) + MMAP_IO_BASE);

    page_alloc.construct((uint8_t *)page_buf, 16384, 4096);

    void *stack_buf = (void *)((uint64_t)host_allocate_pages((KERNEL_STACK_SIZE * 8) / 4096) + MMAP_IO_BASE);

    stack_alloc.construct((uint8_t *)stack_buf, KERNEL_STACK_SIZE * 8, KERNEL_STACK_SIZE);
}

void *host_get_alloc(size_t size)
{
    if (size <= 256)
        return &alloc;

    else if (size > 256 && size <= 4096)
    {
        return &page_alloc;
    }

    return &stack_alloc;
}

void host_log_putc(char c) { com_putc(COM1, c); }
void host_log_write(const char *s)
{
    com_write(COM1, s);
}

const char *host_get_current_task()
{
    if(get_sched())
        return get_sched()->current_task->name;
    return nullptr;
}

void *host_allocate_pages(size_t pages)
{
    return pmm_allocate_zero(pages).value_or(panic_to_lambda("Allocation failed"));
}

void host_free_pages(void *addr, size_t pages)
{
    pmm_free((void *)((uint64_t)addr - MMAP_IO_BASE), pages);
}

void host_map_memory(uintptr_t *pagemap, uint64_t phys_addr, uint64_t virt_addr, uint64_t flags)
{
    vmm_map_page(pagemap, phys_addr, virt_addr, flags);
}