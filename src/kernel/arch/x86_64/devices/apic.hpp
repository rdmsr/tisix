#pragma once
#include <firmware/acpi.hpp>
#include <scheduler.hpp>
#include <tisix/std.hpp>

// TODO: maybe make a base class called "Timer" and then make the apic inherit from it
// this would allow generic use of the timer in the interrupts handler
// Timer* timer = current_timer();
// timer->tick();
// for example

enum LAPIC_REG
{
    LAPIC_CPU_ID = 0x20,
    LAPIC_EOI = 0x0b0,
};

enum APIC_TIMER
{
    APIC_TIMER_REG = 0x320,
    APIC_TIMER_INITIAL_COUNT = 0x380,
    APIC_TIMER_CURRENT_COUNT = 0x390,
    APIC_TIMER_DIVIDER = 0x3e0,
    APIC_LVT_INT_MASKED = 0x10000,
    APIC_LVT_TIMER_MODE_PERIODIC = 0x20000,
};

void apic_initialize(Acpi *acpi);
void apic_eoi();
