#pragma once
#include "abi/syscalls.hpp"
#include <scheduler.hpp>
#include <tasking.hpp>

namespace tisix
{
void loader_new_elf_task(StringView name, uint32_t flags, uint32_t caps = 0, TxEntryType entry = TX_ENTRY_CMAIN, long arg1 = 0, long arg2 = 0, long arg3 = 0);
Scheduler *get_sched();
void loader_init(tisix::Handover *handover);
} // namespace tisix