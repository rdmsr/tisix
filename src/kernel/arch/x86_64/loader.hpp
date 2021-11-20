#pragma once
#include <scheduler.hpp>
#include <tasking.hpp>

namespace tisix
{
void loader_new_elf_task(HandoverModules modules, StringView name, uint32_t flags, Vmm &vmm);
Scheduler *get_sched();
void loader_init();
} // namespace tisix