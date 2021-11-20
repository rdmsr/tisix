#include "pit.hpp"
#include "tisix/assert.hpp"
#include "tisix/std.hpp"
#include <asm.hpp>
#include <devices/apic.hpp>
#include <pmm.hpp>

using namespace tisix;

static uintptr_t base = 0;

uint32_t lapic_read(uint32_t reg)
{

    return volatile_read<uint32_t>(base + MMAP_IO_BASE + reg);
}

void lapic_write(uint32_t reg, uint32_t value)
{
    volatile_write<uint32_t>(base + MMAP_IO_BASE + reg, value);
}

void apic_initialize(Acpi *acpi)
{
    assert(acpi != nullptr);

    base = acpi->lapic_base;

    lapic_write(APIC_TIMER_DIVIDER, 0x3);
    lapic_write(APIC_TIMER_INITIAL_COUNT, 0xFFFFFFFF);
    pit_usleep(10);

    lapic_write(APIC_TIMER_REG, APIC_LVT_INT_MASKED);

    uint32_t tick_in_10ms = 0xFFFFFFFF - lapic_read(APIC_TIMER_CURRENT_COUNT);

    lapic_write(APIC_TIMER_REG, 32 | APIC_LVT_TIMER_MODE_PERIODIC);
    lapic_write(APIC_TIMER_DIVIDER, 0x3);
    lapic_write(APIC_TIMER_INITIAL_COUNT, tick_in_10ms);

    log("Apic timer initialized");
}

void apic_eoi()
{
    lapic_write(LAPIC_EOI, 0);
}