#pragma once
#include <tisix/std.hpp>

typedef int TxResult;

#define SYSCALL(name) extern "C" TxResult tx_sys_##name

SYSCALL(debug)
(const char *args);

#define FOREACH_SYSCALLS(SYSCALL_) \
    SYSCALL_(DEBUG)

typedef enum
{
#define ITER(SYSCALL_) TX_SYS_##SYSCALL_,
    FOREACH_SYSCALLS(ITER)

#undef ITER

        TX_SYS_COUNT
} TxSyscall;

extern "C" TxResult tx_syscall(TxSyscall syscall, void *arg);