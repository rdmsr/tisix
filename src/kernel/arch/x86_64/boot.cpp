#include "gdt.hpp"
#include "idt.hpp"
#include "loader.hpp"
#include "tasking.hpp"
#include "tisix/host.hpp"
#include <devices/apic.hpp>
#include <devices/com.hpp>
#include <devices/pic.hpp>
#include <devices/pit.hpp>
#include <event.hpp>
#include <firmware/acpi.hpp>
#include <pmm.hpp>
#include <scheduler.hpp>
#include <syscalls.hpp>
#include <tisix/gfx/canvas.hpp>
#include <vmm.hpp>

using namespace tisix;

void *operator new(size_t count)
{

    return malloc(count);
}

void splash();

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

    host_alloc_init();
    syscall_init();

    log("Initialized allocators");

    log("usable pages: {} ({} mb)", pmm_get_usable_pages(), (pmm_get_usable_pages() * PAGE_SIZE) / 1024 / 1024);

    log("Modules:");
    for (size_t i = 0; i < handover->modules.size; i++)
    {
        auto module = handover->modules.modules[i];

        log("\t{} ({} bytes)", module.name, module.size);
    }

    events_init();

    loader_init(handover);

    loader_new_elf_task("init", TX_TASK_USER);
    loader_new_elf_task("echo", TX_TASK_USER);

    Handover *new_handover = new Handover();

    memcpy(new_handover, handover, sizeof(Handover));

    loader_new_elf_task("fb", TX_TASK_USER, TX_TASK_NONE, TX_ENTRY_HANDOVER, (long)new_handover);

    get_sched()->_ready = true;

    while (1)
        ;
}
