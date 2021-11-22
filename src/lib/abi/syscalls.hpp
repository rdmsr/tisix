#pragma once
#include <tisix/std.hpp>

typedef int TxResult;

enum TxEventType
{
    TX_EVENT_NONE,
    TX_EVENT_IRQ
};

typedef uint64_t TxIrq;

struct PACKED TxEvent
{
    TxEventType type = TX_EVENT_NONE;

    union
    {
        TxIrq irq;
    };
};

struct PACKED TxMsg
{
    uint32_t from;

    TxEvent event;
};

typedef uint32_t TxFlags;

#define TX_IPC_NONE ((TxFlags)(0))
#define TX_IPC_SEND ((TxFlags)(1 << 1))
#define TX_IPC_RECV ((TxFlags)(1 << 2))
#define TX_FROM_EVENT ((uint32_t)(1 << 2))

struct PACKED TxIpc
{
    TxMsg msg;

    uint32_t to;

    bool received = false;

    TxFlags flags = TX_IPC_SEND;
};

struct PACKED TxMap
{
    uint64_t phys;
    uint64_t virt;
    uint64_t flags;
};

#define SYSCALL(name) extern "C" TxResult tx_sys_##name

SYSCALL(debug)
(const char *args);

SYSCALL(ipc)
(TxIpc *ipc);

SYSCALL(bind)
(TxEvent *event);

SYSCALL(map)
(TxMap *map);

#define FOREACH_SYSCALLS(SYSCALL_) \
    SYSCALL_(DEBUG)                \
    SYSCALL_(IPC)                  \
    SYSCALL_(BIND)                 \
    SYSCALL_(MAP)

typedef enum
{
#define ITER(SYSCALL_) TX_SYS_##SYSCALL_,
    FOREACH_SYSCALLS(ITER)

#undef ITER

        TX_SYS_COUNT
} TxSyscall;

extern "C" TxResult tx_syscall(TxSyscall syscall, uint64_t arg0, uint64_t arg1 = 0, uint64_t arg2 = 0, uint64_t arg3 = 0, uint64_t arg4 = 0);