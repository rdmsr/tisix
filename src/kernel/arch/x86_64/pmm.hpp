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

tisix::Maybe<void *> pmm_allocate(size_t pages);
tisix::Maybe<void *> pmm_allocate_zero(size_t pages);

void pmm_free(void *addr, size_t pages);

void pmm_initialize(tisix::Handover *handover);

void pmm_dump(tisix::Handover *handover);

void pmm_print_bitmap(int n = 10);

uint64_t pmm_get_usable_pages();
