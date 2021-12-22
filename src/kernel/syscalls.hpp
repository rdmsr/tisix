#pragma once
#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <interrupts.hpp>
#include <tisix/std.hpp>

TxResult syscall_dispatch(Stack *stack, TxSyscall sys_number, uint64_t args);

void sys_init(tisix::Handover *handover);

void syscall_set_gs(uintptr_t addr);

void syscall_init(void);
