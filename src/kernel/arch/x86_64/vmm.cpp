#include "tisix/maybe.hpp"
#include <asm.hpp>
#include <tisix/assert.hpp>
#include <vmm.hpp>

using namespace tisix;

uintptr_t *kernel_pagemap = 0;

void vmm_initialize(Handover *handover)
{

    (void)handover;

    kernel_pagemap = (uintptr_t *)pmm_allocate_zero(1).value_or(error_to_lambda("Failed allocation (NULL)"));

    log("Mapping memory...");

    vmm_map_range(kernel_pagemap, 0, 0x8000000, MMAP_KERNEL_BASE, 0b11);
    vmm_map_range(kernel_pagemap, 0, 0x100000000, MMAP_IO_BASE, 0b111);

    // for (auto e : handover->mmap)
    // {
    //     if (e.type == HANDOVER_MMAP_FREE)
    //     {
    //         for (size_t p = 0; p < e.size; p += PAGE_SIZE)
    //             this->map_page(kernel_pagemap, p, p + MMAP_IO_BASE, 0b111);
    //     }
    // }

    vmm_load_pagemap(kernel_pagemap);

    log("initialized vmm");
}

uint64_t *vmm_get_next_level(uint64_t *table, size_t index, uint64_t flags)
{
    if (!(table[index] & 1))
    {
        table[index] = (uint64_t)pmm_allocate_zero(1).value_or(error_to_lambda("Failed allocation (NULL)"));
        table[index] |= flags;
    }

    return (uint64_t *)((table[index] & ~(0x1ff)) + MMAP_IO_BASE);
}

void vmm_load_pagemap(uint64_t *pagemap)
{
    asm_write_cr3((uint64_t)pagemap);
}

void vmm_flush_tlb(void *addr)
{
    asm volatile("invlpg (%0)"
                 :
                 : "r"(addr));
}

void vmm_map_page(uint64_t *pagemap, uintptr_t physical_address, uintptr_t virtual_address, uint64_t flags)
{
    size_t level4 = PML_ENTRY(virtual_address, 39);
    size_t level3 = PML_ENTRY(virtual_address, 30);
    size_t level2 = PML_ENTRY(virtual_address, 21);
    size_t level1 = PML_ENTRY(virtual_address, 12);

    uint64_t *pml3 = vmm_get_next_level(pagemap, level4, flags);
    uint64_t *pml2 = vmm_get_next_level(pml3, level3, flags);
    uint64_t *pml1 = vmm_get_next_level(pml2, level2, flags);

    pml1[level1] = physical_address | flags;

    vmm_flush_tlb((void *)virtual_address);
}

void vmm_unmap_page(uint64_t *pagemap, uintptr_t virtual_address, uint64_t flags)
{
    size_t level4 = PML_ENTRY(virtual_address, 39);
    size_t level3 = PML_ENTRY(virtual_address, 30);
    size_t level2 = PML_ENTRY(virtual_address, 21);
    size_t level1 = PML_ENTRY(virtual_address, 12);

    uint64_t *pml3 = vmm_get_next_level(pagemap, level4, flags);
    uint64_t *pml2 = vmm_get_next_level(pml3, level3, flags);
    uint64_t *pml1 = vmm_get_next_level(pml2, level2, flags);

    pml1[level1] = 0;
}

void vmm_map_range(uint64_t *pagemap, uint64_t start, uint64_t end, uint64_t offset, uint64_t flags)
{

    for (uintptr_t i = start; i < end; i += PAGE_SIZE)
    {
        vmm_map_page(pagemap, i, i + offset, flags);
    }
}

uint64_t *vmm_get_kernel_pagemap()
{
    return kernel_pagemap;
}
