#pragma once
#include <abi/syscalls.hpp>
#include <tisix/std.hpp>

TxResult syscall_dispatch(TxSyscall sys_number, uint64_t args);