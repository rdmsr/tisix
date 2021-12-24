#include "abi/syscalls.hpp"
#include "pmm.hpp"
#include "tisix/handover.hpp"
#include "tisix/mem.hpp"
#include "tisix/std.hpp"
#include "vmm.hpp"
#include <elf.hpp>
#include <loader.hpp>
#include <tisix/assert.hpp>
#include <tisix/host.hpp>

using namespace tisix;

Scheduler *scheduler = nullptr;
tisix::Handover *handover;

Scheduler *tisix::get_sched() { return scheduler; }

void tisix::loader_init(tisix::Handover *_handover)
{

    scheduler = new Scheduler(20);
    handover = new Handover;

    memcpy(handover, _handover, sizeof(Handover));
}

uint64_t elf_load_program(uint64_t elf_base, Task *task)
{
    auto elf_header = (Elf64Header *)elf_base;

    Elf64ProgramHeader *prog_header = (Elf64ProgramHeader *)(elf_base + elf_header->program_header_table_file_offset);

    for (size_t i = 0; i < elf_header->program_header_table_entry_count; i++)
    {

        if (prog_header->type == ELF_PROGRAM_HEADER_LOAD)
        {
            auto new_addr = host_allocate_pages(ALIGN_UP(prog_header->memory_size, PAGE_SIZE) / 4096);

            for (size_t j = 0; j < ALIGN_UP(prog_header->memory_size, PAGE_SIZE) / 4096; j++)
            {

                host_map_memory(task->pagemap, j * PAGE_SIZE + (uint64_t)new_addr, j * PAGE_SIZE + prog_header->virtual_address, 0b111);
            }

            tisix::memcpy((void *)((uint64_t)new_addr + MMAP_IO_BASE), (void *)(elf_base + prog_header->file_offset), prog_header->file_size);
            tisix::memset((void *)((uint64_t)new_addr + MMAP_IO_BASE + prog_header->file_size), 0, prog_header->memory_size - prog_header->file_size);
        }

        prog_header = (Elf64ProgramHeader *)((uint8_t *)prog_header + elf_header->program_header_table_entry_size);
    }

    return elf_header->entry;
}

static uint64_t counter = 2 << 20;

void *allocate_user_page(uint64_t *pagemap)
{
    auto addr = host_allocate_pages(1);

    host_map_memory(pagemap, (uint64_t)addr, counter, 0b111);

    auto ret = counter;

    counter += PAGE_SIZE;

    return (void *)ret;
}

void tisix::loader_new_elf_task(StringView name, uint32_t flags, uint32_t caps, TxEntryType type, long arg1, long arg2, long arg3)
{

    Task *new_task = new Task(name, flags, caps);

    new_task->start(0);

    get_sched()->_ready = false;

    Elf64Header *header = nullptr;

    auto modules = handover->modules;

    for (size_t i = 0; i < modules.size; i++)
    {
        auto module = modules.modules[i];

        if (module.name == name)
        {
            header = (Elf64Header *)(module.addr + MMAP_IO_BASE);
        }
    }

    assert(header != nullptr);

    assert(elf_validate(header) == true);

    new_task->stack.rdi = type;

    new_task->stack.rsi = arg1;

    if (type == TX_ENTRY_HANDOVER)
    {

        handover = (Handover *)arg1;

        void *buf = allocate_user_page(new_task->pagemap);

        host_map_memory(new_task->pagemap, (uint64_t)handover - MMAP_IO_BASE, (uint64_t)buf, 0b111);

        uint64_t new_addr = 0;

        for (size_t i = 0; i < ALIGN_UP(handover->framebuffer.height * handover->framebuffer.pitch, PAGE_SIZE) / PAGE_SIZE; i++)
        {
            void *fb_buf = allocate_user_page(new_task->pagemap);

            host_map_memory(new_task->pagemap, (uint64_t)handover->framebuffer.addr - MMAP_IO_BASE + i * PAGE_SIZE, (uint64_t)fb_buf, 0b111);

            if (i == 0)
                new_addr = (uint64_t)fb_buf;
        }

        handover->framebuffer.addr = new_addr;
        new_task->stack.rsi = (uint64_t)buf;
    }

    new_task->stack.rdx = arg2;
    new_task->stack.rcx = arg3;
    new_task->stack.rip = elf_load_program((uint64_t)header, new_task);

    log("Loaded task {} with rip {#p}", name, new_task->stack.rip);

    get_sched()->add_task(new_task);
}
