#include "gdt.hpp"
#include <tisix/mem.hpp>

using namespace tisix;

extern "C" void gdt_update(uint64_t);

static GdtPointer gdtr;
static Gdt gdt = {};

static Tss tss = {
    .reserved = 0,
    .rsp = {},
    .reserved0 = 0,
    .ist = {},
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
    .iopb_offset = 0,
};

TssEntry make_tss_entry(uintptr_t tss)
{
    return (TssEntry){
        .length = sizeof(TssEntry),
        .base_low16 = static_cast<uint16_t>(tss & 0xffff),
        .base_mid8 = static_cast<uint8_t>((tss >> 16) & 0xff),
        .flags1 = 0b10001001,
        .flags2 = 0,
        .base_high8 = static_cast<uint8_t>((tss >> 24) & 0xff),
        .base_upper32 = static_cast<uint32_t>(tss >> 32),
        .reserved = 0,
    };
}

GdtDescriptor gdt_entry(uint32_t base, uint32_t limit, uint8_t granularity, uint8_t flags)
{
    return (GdtDescriptor){
        .limit16 = (uint16_t)(limit & 0xffff),
        .base16 = (uint16_t)(base & 0xffff),
        .base23 = (uint8_t)(((base) >> 16) & 0xff),
        .flags = flags,
        .limit19 = static_cast<uint8_t>((limit >> 16) & 0x0f),
        .granularity = granularity,
        .base31 = (uint8_t)((base >> 24) & 0xff),
    };
}

GdtDescriptor gdt_entry_simple(uint8_t flags, uint8_t granularity)
{
    return gdt_entry(0, 0, granularity, flags);
}

extern "C" void tss_update();

void gdt_initialize(uint8_t *kernel_stack)
{
    gdtr.base = (uint64_t)&gdt;

    gdtr.limit = sizeof(Gdt) - 1;

    gdt.entries[0] = gdt_entry(0, 0, 0, 0);

    gdt.entries[GDT_KERNEL_CODE] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE, GDT_LONG_MODE_GRANULARITY);
    gdt.entries[GDT_KERNEL_DATA] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE, 0);

    gdt.entries[GDT_USER_DATA] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_USER, 0);
    gdt.entries[GDT_USER_CODE] = gdt_entry_simple(GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE | GDT_EXECUTABLE | GDT_USER, GDT_LONG_MODE_GRANULARITY);

    gdt.tss = make_tss_entry((uintptr_t)&tss);

    memset(&tss, 0, sizeof(tss));

    tss.rsp[0] = (uintptr_t)kernel_stack + sizeof(kernel_stack);
    tss.ist[1] = 0;

    tss.iopb_offset = sizeof(tss);

    gdt_update((uint64_t)&gdtr);

    tss_update();

    log("initialized gdt");
}
