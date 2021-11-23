#include "abi/syscalls.hpp"
#include "tisix/alloc.hpp"
#include "tisix/arch.hpp"
#include "tisix/maybe.hpp"
#include <loader.hpp>
#include <pmm.hpp>
#include <tasking.hpp>
#include <tisix/assert.hpp>

using namespace tisix;

static uint64_t curr_id = 0;

Task::Task(StringView name, uint8_t m_flags)
{
    this->name = name;
    this->id = curr_id;

    curr_id++;

    this->flags = m_flags;
    this->return_value = 0;
    this->index = get_sched()->tasks.size;

    this->ipc_buffer = new TxIpc;

    auto stack = (uintptr_t)malloc(KERNEL_STACK_SIZE);

    this->sp = stack + KERNEL_STACK_SIZE;

    if (flags & TX_USER)
    {
        this->pagemap = (uint64_t *)((uint64_t)host_allocate_pages(1) + MMAP_KERNEL_BASE);

        auto n_pmap = (uint64_t *)((uint64_t)vmm_get_kernel_pagemap() + MMAP_KERNEL_BASE);

        for (auto i = 256; i < 512; i++)
        {
            pagemap[i] = n_pmap[i];
        }

        for (size_t i = 0; i < (KERNEL_STACK_SIZE / 4096); i++)
        {

            auto phys_addr = i * PAGE_SIZE + ALIGN_DOWN((stack - MMAP_IO_BASE), PAGE_SIZE);
            auto virt_addr = i * PAGE_SIZE + ALIGN_DOWN((USER_STACK_BASE - KERNEL_STACK_SIZE), PAGE_SIZE);

            host_map_memory(pagemap, phys_addr, virt_addr, 0b111);
        }
    }
}

void Task::start(uintptr_t ip)
{
    Stack regs = {};

    regs.rip = ip;
    regs.intno = 0;
    regs.err = 0;
    regs.rbp = 0;

    regs.rflags = 0x202;

    if (flags & TX_USER)
    {
        regs.cs = 0x23;
        regs.ss = 0x1b;
        regs.rsp = USER_STACK_BASE;
    }

    else
    {
        regs.cs = 0x8;
        regs.ss = 0x10;
        pagemap = vmm_get_kernel_pagemap();
    }

    this->stack = regs;

    log("Started task {}({})...", name, id);
}
