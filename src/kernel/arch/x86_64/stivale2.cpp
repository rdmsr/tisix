#include "stivale2.hpp"
#include <pmm.hpp>
#include <stdint.h>
#include <tisix/arch.hpp>
#include <tisix/handover.hpp>
using namespace tisix;

uint8_t stack[KERNEL_STACK_SIZE];

extern "C" void _kstart(struct stivale2_struct const *info);

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = 0,
    },
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0,
    .unused = 0,
};

[[gnu::section(".stivale2hdr"), gnu::used]] static struct stivale2_header stivale_hdr = {
    .entry_point = (uintptr_t)_kstart,
    .stack = (uintptr_t)stack + sizeof(stack),
    .flags = (1 << 1),
    .tags = (uintptr_t)&framebuffer_hdr_tag,
};

void *stivale2_get_tag(struct stivale2_struct const *stivale2_struct, uint64_t id)
{
    struct stivale2_tag *current_tag = (struct stivale2_tag *)stivale2_struct->tags;

    while (current_tag)
    {
        if (current_tag->identifier == id)
        {
            return current_tag;
        }

        current_tag = (struct stivale2_tag *)current_tag->next;
    }

    return nullptr;
}

static HandoverMmapType stivale_mmap_type_to_handover_type(int type)
{
    switch (type)
    {

    case STIVALE2_MMAP_USABLE:
        return HANDOVER_MMAP_FREE;

    case STIVALE2_MMAP_KERNEL_AND_MODULES:
        return HANDOVER_MMAP_MODULE;

    case STIVALE2_MMAP_ACPI_NVS:
    case STIVALE2_MMAP_RESERVED:
    case STIVALE2_MMAP_BAD_MEMORY:
        return HANDOVER_MMAP_RESERVED;

    case STIVALE2_MMAP_BOOTLOADER_RECLAIMABLE:
    case STIVALE2_MMAP_ACPI_RECLAIMABLE:
        return HANDOVER_MMAP_RECLAIMABLE;

    case STIVALE2_MMAP_FRAMEBUFFER:
        return HANDOVER_MMAP_FRAMEBUFFER;

    default:
        return HANDOVER_MMAP_RESERVED;
    }
}

static void stivale_mmap_to_handover_mmap(Handover *target, struct stivale2_struct_tag_memmap *memory_map)
{
    target->mmap.size = memory_map->entries;

    for (size_t i = 0; i < memory_map->entries; i++)
    {
        HandoverMmapEntry *entry = &target->mmap.entries[i];

        entry->size = memory_map->memmap[i].length;
        entry->base = memory_map->memmap[i].base;
        entry->type = stivale_mmap_type_to_handover_type(memory_map->memmap[i].type);
    }
}

static void stivale_modules_to_handover_modules(Handover *target, struct stivale2_struct_tag_modules *modules)
{
    target->modules.size = modules->module_count;

    for (size_t i = 0; i < modules->module_count; i++)
    {

        target->modules.modules[i].addr = modules->modules[i].begin - MMAP_IO_BASE;
        target->modules.modules[i].size = ALIGN_UP(modules->modules[i].end - modules->modules[i].begin, PAGE_SIZE);
        target->modules.modules[i].name = modules->modules[i].string;
    }
}

static void stivale_fb_to_handover_fb(Handover *target, struct stivale2_struct_tag_framebuffer *fb)
{
    target->framebuffer.addr = fb->framebuffer_addr;
    target->framebuffer.bpp = fb->framebuffer_bpp;
    target->framebuffer.height = fb->framebuffer_height;
    target->framebuffer.pitch = fb->framebuffer_pitch;
    target->framebuffer.width = fb->framebuffer_width;
}

extern "C" void _kstart(struct stivale2_struct const *info)
{
    Handover handover;

    void *memory_map = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_MEMMAP_ID);

    if (memory_map)
        stivale_mmap_to_handover_mmap(&handover, (struct stivale2_struct_tag_memmap *)memory_map);

    void *modules = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_MODULES_ID);
    if (modules)
        stivale_modules_to_handover_modules(&handover, (stivale2_struct_tag_modules *)modules);

    void *rsdp = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_RSDP_ID);
    if (rsdp)
        handover.rsdp = ((struct stivale2_struct_tag_rsdp *)rsdp)->rsdp;

    void *framebuffer = stivale2_get_tag(info, STIVALE2_STRUCT_TAG_FRAMEBUFFER_ID);
    if (framebuffer)
        stivale_fb_to_handover_fb(&handover, (stivale2_struct_tag_framebuffer *)framebuffer);

    handover.framebuffer.present = true;

    arch_entry_main(&handover);
}