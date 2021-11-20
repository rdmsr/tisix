#include "pmm.hpp"
#include "tisix/arch.hpp"
#include "tisix/bitmap.hpp"
#include "tisix/handover.hpp"
#include "tisix/mem.hpp"
#include <tisix/assert.hpp>
#include <tisix/host.hpp>

using namespace tisix;

static Bitmap bitmap;
static uintptr_t usable_pages = 0;
static uintptr_t highest_page = 0;

uint64_t pmm_get_usable_pages() { return usable_pages; };

static Maybe<size_t> get_bitmap_size(Handover *handover)
{
    uintptr_t top = 0;

    for (size_t i = 0; i < handover->mmap.size; i++)
    {
        auto *entry = &handover->mmap.entries[i];

        if (entry->type != HANDOVER_MMAP_FREE && entry->type != HANDOVER_MMAP_MODULE && entry->type != HANDOVER_MMAP_RECLAIMABLE)
            continue;

        top = entry->size + entry->base;

        if (top > highest_page)
        {
            highest_page = top;
        }
    }

    size_t bitmap_size = ALIGN_UP(ALIGN_DOWN(highest_page, PAGE_SIZE) / PAGE_SIZE / 8, PAGE_SIZE);

    if (bitmap_size > 0)
    {
        return Just(bitmap_size);
    }

    return Nothing;
}

void pmm_initialize(Handover *handover)
{

    auto bitmap_size = get_bitmap_size(handover).value_or(panic_to_lambda("Could not get bitmap size"));

    for (size_t i = 0; i < handover->mmap.size; i++)
    {
        auto *e = &handover->mmap.entries[i];

        if (e->type == HANDOVER_MMAP_FREE && e->size >= bitmap_size)
        {
            log("Allocated memory bitmap at {#x}-{#x}", e->base, e->base + bitmap_size - 1);

            bitmap = Bitmap{(uint8_t *)(e->base + MMAP_IO_BASE), bitmap_size};

            e->base += bitmap_size;
            e->size -= bitmap_size;

            break;
        }
    }

    bitmap.fill(0xff);

    log("bitmap is {} kib long", bitmap_size / 1024);

    for (auto entry : handover->mmap)
    {
        if (entry.type == HANDOVER_MMAP_FREE)
        {
            pmm_free((void *)entry.base, entry.size / PAGE_SIZE);
        }
    }
}

static void set_page(void *addr)
{

    bitmap.set((size_t)addr / PAGE_SIZE);
    usable_pages--;
}

static void set_pages(void *addr, size_t page_count)
{
    for (size_t i = 0; i < page_count; i++)
    {
        set_page((void *)((uintptr_t)addr + (i * PAGE_SIZE)));
    }
}

static void clear_page(void *addr)
{

    bitmap.clear((size_t)addr / PAGE_SIZE);
    usable_pages++;
}

void pmm_free(void *addr, size_t pages)
{
    for (size_t i = 0; i < pages; i++)
    {
        clear_page((void *)((uintptr_t)addr + (i * PAGE_SIZE)));
    }
}

Maybe<void *> pmm_allocate(size_t pages)
{
    assert(pages > 0);
    assert(usable_pages > 0);
    assert(bitmap.size > pages);

    for (size_t i = 0; i < highest_page / PAGE_SIZE; i++)
    {
        for (size_t j = 0; j < pages; j++)
        {
            if (bitmap[i])
            {
                break;
            }

            else if (j == pages - 1)
            {
                void *ret = (void *)(i * PAGE_SIZE);

                set_pages(ret, pages);

                return Just(ret);
            }
        }
    }

    return Nothing;
}

Maybe<void *> pmm_allocate_zero(size_t pages)
{
    void *ret = pmm_allocate(pages).value_or(panic_to_lambda("Failed allocating memory"));

    memset((void *)((uintptr_t)ret + MMAP_IO_BASE), 0, pages * PAGE_SIZE);

    return Just(ret);
}

static const char *get_memmap_entry_type(int type)
{
    switch (type)
    {
    case HANDOVER_MMAP_FREE:
        return "Free";
    case HANDOVER_MMAP_RESERVED:
        return "Reserved";
    case HANDOVER_MMAP_RECLAIMABLE:
        return "Reclaimable";
    case HANDOVER_MMAP_MODULE:
        return "Module";
    case HANDOVER_MMAP_FRAMEBUFFER:
        return "Framebuffer";
    default:
        return "???";
    }
}

void pmm_dump(Handover *handover)
{
    log("Loading mmap");

    for (auto entry : handover->mmap)
    {
        log("Entry {#x}-{#x} length={#x} type={}", entry.base, entry.base + entry.size, entry.size, get_memmap_entry_type(entry.type));
    }
}

void pmm_print_bitmap(int n)
{
    if (n == 0)
    {
        for (size_t i = 0; i < bitmap.size; i++)
        {
            fmt_stream(host_log_write, "{}", (int)bitmap[i]);
        }
    }

    else
    {
        for (int i = 0; i < n; i++)
        {
            fmt_stream(host_log_write, "{}", (int)bitmap[i]);
        }
    }
}
