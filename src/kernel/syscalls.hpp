#pragma once
#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/std.hpp>

TxResult syscall_dispatch(TxSyscall sys_number, uint64_t args);

void sys_init(tisix::Handover *handover);