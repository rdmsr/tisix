#pragma once
#include <scheduler.hpp>
#include <tasking.hpp>

namespace tisix
{
void loader_new_elf_task(StringView name, uint32_t flags, TxEntryType entry, long arg1, long arg2, long arg3);
Scheduler *get_sched();
void loader_init(tisix::Handover *handover);
} // namespace tisix