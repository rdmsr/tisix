#pragma once
#include "tisix/handover.hpp"
#include <pmm.hpp>

#define PML_ENTRY(addr, offset) (size_t)(addr & ((uintptr_t)0x1ff << offset)) >> offset;

class Vmm
{
public:
    Vmm(MemoryAllocator *pmm, tisix::Handover *);

    void map_page(uint64_t *pagemap, uintptr_t physical_address, uintptr_t virtual_address, uint64_t flags);
    void map_range(uint64_t *pagemap, uint64_t start, uint64_t end, uint64_t offset, uint64_t flags);

    void unmap_page(uint64_t *pagemap, uintptr_t virtual_address, uint64_t flags);

    void load_pagemap(uint64_t *pagemap);

    uint64_t *get_kernel_pagemap() { return kernel_pagemap; };

private:
    MemoryAllocator *pmm;
    tisix::Handover *handover;
    uint64_t *kernel_pagemap;

private:
    uint64_t *get_next_level(uint64_t *table, size_t index, uint64_t flags);
    void flush_tlb(void *addr);
};
