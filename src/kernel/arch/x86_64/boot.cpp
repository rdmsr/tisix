#include "gdt.hpp"
#include "idt.hpp"
#include "interrupts.hpp"
#include "loader.hpp"
#include "stivale2.hpp"
#include "tasking.hpp"
#include "tisix/alloc.hpp"
#include "tisix/assert.hpp"
#include "tisix/handover.hpp"
#include "tisix/maybe.hpp"
#include "tisix/std.hpp"
#include <devices/apic.hpp>
#include <devices/com.hpp>
#include <devices/pic.hpp>
#include <devices/pit.hpp>
#include <event.hpp>
#include <firmware/acpi.hpp>
#include <pmm.hpp>
#include <scheduler.hpp>
#include <syscalls.hpp>
#include <tisix/arch.hpp>
#include <tisix/vec.hpp>
#include <vmm.hpp>

using namespace tisix;

void splash();

void *operator new(size_t size)
{
    return malloc(size);
}

extern uint8_t stack[KERNEL_STACK_SIZE];

void arch_entry_main(Handover *handover)
{
    com_initialize(COM1);

    splash();

    gdt_initialize(stack);

    idt_initialize();

    pic_initialize();

    pit_initialize(1000);

    Acpi acpi(handover);

    apic_initialize(&acpi);

    pmm_initialize(handover);

    pmm_dump(handover);

    asm_cli();

    vmm_initialize(handover);

    asm_sti();

    log("usable pages: {} ({} mb)", pmm_get_usable_pages(), (pmm_get_usable_pages() * PAGE_SIZE) / 1024 / 1024);

    log("Modules:");
    for (size_t i = 0; i < handover->modules.size; i++)
    {
        auto module = handover->modules.modules[i];

        log("\t{} ({} bytes)", module.name, module.size);
    }

    events_init();

    loader_init();

    loader_new_elf_task(handover->modules, "echo", TX_USER, (void *)handover);
    loader_new_elf_task(handover->modules, "fb", TX_USER, (void *)handover);

    get_sched()->_ready = true;

    while (1)
        ;
}
