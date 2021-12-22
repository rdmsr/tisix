#pragma once
#include "tisix/handover.hpp"
#include <pmm.hpp>

#define PML_ENTRY(addr, offset) (size_t)(addr & ((uintptr_t)0x1ff << offset)) >> offset;

void vmm_initialize(tisix::Handover *handover);

void vmm_map_page(uint64_t *pagemap, uintptr_t phys_addr, uintptr_t virt_addr, uint64_t flags);
void vmm_map_range(uint64_t *pagemap, uint64_t start, uint64_t end, uint64_t offset, uint64_t flags);
void vmm_unmap_page(uint64_t *pagemap, uintptr_t virt_addr, uint64_t flags);
void vmm_load_pagemap(uint64_t *pagemap);
uint64_t *vmm_get_kernel_pagemap();
void vmm_flush_tlb(void *addr);
