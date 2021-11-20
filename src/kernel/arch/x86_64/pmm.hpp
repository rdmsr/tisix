#pragma once
#include "tisix/stream.hpp"
#include <tisix/arch.hpp>
#include <tisix/bitmap.hpp>
#include <tisix/handover.hpp>
#include <tisix/log.hpp>
#include <tisix/maybe.hpp>

#define PAGE_SIZE 4096
#define MMAP_IO_BASE ((uintptr_t)0xffff800000000000)
#define MMAP_KERNEL_BASE ((uintptr_t)0xffffffff80000000)

class Pmm : public MemoryAllocator
{
public:
    tisix::Maybe<void *> allocate(size_t pages) override;

    tisix::Maybe<void *> allocate_zero(size_t pages) override;

    void free(void *addr, size_t pages) override;

    Pmm(tisix::Handover *handover);

    void print_bitmap(tisix::Stream<const char *> *stream, int n = 0);

    void dump();

    size_t usable_pages = 0;

private:
    tisix::Bitmap bitmap;
    tisix::Handover *handover;
    uintptr_t highest_page;

    tisix::Maybe<size_t> get_bitmap_size();

    void set_page(void *addr);
    void set_pages(void *addr, size_t pages);
    void clear_page(void *addr);
};