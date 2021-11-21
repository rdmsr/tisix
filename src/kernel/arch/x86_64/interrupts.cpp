#include "interrupts.hpp"
#include "asm.hpp"
#include "event.hpp"
#include "pmm.hpp"
#include "scheduler.hpp"
#include "tisix/maybe.hpp"
#include "vmm.hpp"
#include <common.hpp>
#include <devices/apic.hpp>
#include <devices/pic.hpp>
#include <loader.hpp>
#include <syscalls.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>

static const char *exceptions[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved"};

static void backtrace(void (*s)(const char *), uintptr_t rbp, uint64_t rip)
{
    Stack *stackframe = (Stack *)rbp;

    fmt_stream(s, "*** Backtrace ***\n");
    fmt_stream(s, "\t- {#p}\n", rip);

    while (stackframe)
    {
        fmt_stream(s, "\t- {#p}\n", stackframe->rip);
        stackframe = (Stack *)stackframe->rbp;
    }
}
void interrupt_error_handler(Stack *stackframe)
{
    uint64_t cr0 = asm_read_cr0();
    uint64_t cr2 = asm_read_cr2();
    uint64_t cr3 = asm_read_cr3();

    auto debug = host_log_write;

    fmt_stream(debug, "______________________\n");
    fmt_stream(debug, "< Wait, that's illegal >\n");
    fmt_stream(debug, " ---------------------- \n");
    fmt_stream(debug, "        \\   ^__^\n");
    fmt_stream(debug, "         \\  (xx)\\______\n");
    fmt_stream(debug, "            (__)\\       )\\/\\\n");
    fmt_stream(debug, "            U  ||----w |\n");
    fmt_stream(debug, "                ||     ||\n");
    fmt_stream(debug, "---- \033[1;31m{}\033[0m ({#x}) on cpu0 ({#x}) ---- \n", exceptions[stackframe->intno], stackframe->intno, stackframe->err);
    fmt_stream(debug, "RAX={#p} RBX={#p} RCX={#p} RDX={#p}\n", stackframe->rax, stackframe->rbx, stackframe->rcx, stackframe->rdx);
    fmt_stream(debug, "RSI={#p} RDI={#p} RBP={#p} RSP={#p}\n", stackframe->rsi, stackframe->rdi, stackframe->rbp, stackframe->rsp);
    fmt_stream(debug, "R8={#p}  R9={#p}  R10={#p} R11={#p}\n", stackframe->r8, stackframe->r9, stackframe->r10, stackframe->r11);
    fmt_stream(debug, "R12={#p} R13={#p} R14={#p} R15={#p}\n", stackframe->r12, stackframe->r13, stackframe->r14, stackframe->r15);
    fmt_stream(debug, "CR0={#p} CR2={#p} CR3={#p} RIP={#p}\n", cr0, cr2, cr3, stackframe->rip);
    backtrace(debug, stackframe->rbp, stackframe->rip);
}

Task *prev_task;

extern "C" uint64_t interrupts_handler(uint64_t rsp)
{
    Stack *stackframe = (Stack *)rsp;

    if (stackframe->intno >= 32 && stackframe->intno <= 48)
    {
        tisix::trigger_event((TxEvent){.type = TX_EVENT_IRQ, .irq = stackframe->intno - 32});
    }

    if (stackframe->intno < 32)
    {
        interrupt_error_handler(stackframe);
        arch_hang();
    }

    else if (stackframe->intno == 32)
    {
        if (tisix::get_sched())
        {
            if (tisix::get_sched()->ready())
            {
                if (prev_task)
                    prev_task->stack = *stackframe;

                auto result = tisix::get_sched()->tick();

                if (result.success)
                {
                    auto task = result.value;

                    prev_task = task;

                    tisix::get_sched()->current_task = task;

                    *stackframe = task->stack;

                    if (task->flags & TX_USER)
                    {
                        asm_write_cr3((uint64_t)task->pagemap - MMAP_KERNEL_BASE);
                    }

                    else
                    {
                        asm_write_cr3((uint64_t)task->pagemap);
                    }
                }
            }
        }
    }

    else if (stackframe->intno == 0x42)
    {
        syscall_dispatch((TxSyscall)stackframe->rbx, stackframe->rcx);
    }

    apic_eoi();
    return rsp;
}
