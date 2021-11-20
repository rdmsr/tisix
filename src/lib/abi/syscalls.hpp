#pragma once
#include <tisix/std.hpp>

typedef int TxResult;

struct PACKED TxIpc
{
    uint32_t from;
    uint32_t to;

    bool received = false;

    bool send = true;

    int number = 0;
};

#define SYSCALL(name) extern "C" TxResult tx_sys_##name

SYSCALL(debug)
(const char *args);

SYSCALL(ipc)
(TxIpc *ipc);

#define FOREACH_SYSCALLS(SYSCALL_) \
    SYSCALL_(DEBUG)                \
    SYSCALL_(IPC)

typedef enum
{
#define ITER(SYSCALL_) TX_SYS_##SYSCALL_,
    FOREACH_SYSCALLS(ITER)

#undef ITER

        TX_SYS_COUNT
} TxSyscall;

extern "C" TxResult tx_syscall(TxSyscall syscall, void *arg);