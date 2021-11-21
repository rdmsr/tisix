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

Scheduler *tisix::get_sched() { return scheduler; }

void tisix::loader_init()
{
    scheduler = new Scheduler(50);
}

uint64_t elf_load_program(Elf64Header *elf_header, Task *task)
{
    Elf64ProgramHeader *prog_header = (Elf64ProgramHeader *)((uint8_t *)elf_header + elf_header->program_header_table_file_offset);

    for (size_t i = 0; i < elf_header->program_header_table_entry_count; i++)
    {
        if (prog_header->type == ELF_PROGRAM_HEADER_LOAD)
        {
            auto new_addr = host_allocate_pages(ALIGN_UP(prog_header->memory_size, PAGE_SIZE) / 4096);

            for (size_t j = 0; j < ALIGN_UP(prog_header->memory_size, PAGE_SIZE) / 4096; j++)
            {
                host_map_memory(task->pagemap, j * PAGE_SIZE + (uint64_t)new_addr, j * PAGE_SIZE + prog_header->virtual_address, 0b111);
            }

            tisix::memcpy((void *)((uint64_t)new_addr + MMAP_IO_BASE), (void *)((uint64_t)elf_header + prog_header->file_offset), prog_header->file_size);
            tisix::memset((void *)((uint64_t)new_addr + MMAP_IO_BASE + prog_header->file_size), 0, prog_header->memory_size - prog_header->file_size);
        }

        prog_header += elf_header->program_header_table_entry_size;
    }

    return elf_header->entry;
}

void tisix::loader_new_elf_task(HandoverModules modules, StringView name, uint32_t flags, void *args)
{
    Task *new_task = new Task(name, flags);
    new_task->start(0);

    get_sched()->_ready = false;

    Elf64Header *header = nullptr;

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

    auto mem = malloc(sizeof(Handover));

    memcpy(mem, args, sizeof(Handover));

    new_task->stack.rip = elf_load_program(header, new_task);

    new_task->stack.rdi = (uint64_t)mem;

    get_sched()->add_task(new_task);
}