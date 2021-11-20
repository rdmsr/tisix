#pragma once
#include <tisix/std.hpp>
#include <tisix/string_view.hpp>

namespace tisix
{

enum HandoverMmapType
{
    HANDOVER_MMAP_FREE,
    HANDOVER_MMAP_RESERVED,
    HANDOVER_MMAP_MODULE,
    HANDOVER_MMAP_RECLAIMABLE,
    HANDOVER_MMAP_FRAMEBUFFER
};

struct HandoverMmapEntry
{
    uintptr_t base;
    size_t size;
    HandoverMmapType type;
};

struct HandoverMmap
{
    size_t size;
    HandoverMmapEntry entries[64];
    HandoverMmapEntry *begin() { return &entries[0]; };
    HandoverMmapEntry *end() { return &entries[size]; };
};

struct HandoverFramebuffer
{
    bool present;
    uintptr_t addr;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
};

struct HandoverModule
{
    size_t size;
    uintptr_t addr;
    StringView name;
};

struct HandoverModules
{
    size_t size;
    HandoverModule modules[16];
};

struct Handover
{
    HandoverFramebuffer framebuffer;
    HandoverMmap mmap;
    HandoverModules modules;
    uintptr_t rsdp;
};

}; // namespace tisix