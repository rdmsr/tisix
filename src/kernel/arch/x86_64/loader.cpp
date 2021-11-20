#include "pmm.hpp"
#include "tisix/mem.hpp"
#include "tisix/std.hpp"
#include <elf.hpp>
#include <loader.hpp>
#include <tisix/assert.hpp>

using namespace tisix;

Scheduler *scheduler = nullptr;

Scheduler *tisix::get_sched() { return scheduler; }

void tisix::loader_init()
{
    scheduler = new Scheduler(10);
}

uint64_t elf_load_program(Elf64Header *elf_header, Vmm &vmm, Task *task)
{
    Elf64ProgramHeader *prog_header = (Elf64ProgramHeader *)((uint8_t *)elf_header + elf_header->program_header_table_file_offset);

    for (size_t i = 0; i < elf_header->program_header_table_entry_count; i++)
    {
        if (prog_header->type == ELF_PROGRAM_HEADER_LOAD)
        {
            auto new_addr = get_arch()->allocator->allocate(ALIGN_UP(prog_header->memory_size, PAGE_SIZE) / 4096).value;

            vmm.map_page(task->pagemap, i + (uint64_t)new_addr, i + prog_header->virtual_address, 0b111);

            tisix::memcpy((void *)((uint64_t)new_addr + MMAP_IO_BASE), (void *)((uint64_t)elf_header + prog_header->file_offset), prog_header->file_size);
            tisix::memset((void *)((uint64_t)new_addr + MMAP_IO_BASE + prog_header->file_size), 0, prog_header->memory_size - prog_header->file_size);
        }

        prog_header += elf_header->program_header_table_entry_size;
    }

    return elf_header->entry;
}

void tisix::loader_new_elf_task(HandoverModules modules, StringView name, uint32_t flags, Vmm &vmm)
{
    Task *new_task = new Task(vmm, name, flags);
    new_task->start(0);

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

    new_task->stack.rip = elf_load_program(header, vmm, new_task);

    get_sched()->add_task(new_task);
}