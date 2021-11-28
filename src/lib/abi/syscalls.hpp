#pragma once
#include <tisix/std.hpp>

typedef int TxResult;

#define TX_TASK_NONE (uint8_t)(0)
#define TX_TASK_USER (uint8_t)(1 << 0)

#define TX_CAP_IO (uint8_t)(1 << 0)

enum TxEventType
{
    TX_EVENT_NONE,
    TX_EVENT_IRQ
};

enum TxEntryType
{
    TX_ENTRY_HANDOVER,
    TX_ENTRY_CMAIN,
};

enum TxMsgType
{
    TX_MSG_NONE,
    TX_MSG_DATA,
    TX_MSG_EVENT,
    TX_MSG_REQUEST,
    TX_MSG_RESPONSE_DATA,
};

typedef uint64_t TxData;

struct PACKED TxEvent
{
    TxEventType type = TX_EVENT_NONE;

    union
    {
        TxData irq;
    };
};

struct PACKED TxMsg
{
    uint32_t from;

    TxMsgType type = TX_MSG_EVENT;

    union
    {
        TxEvent event;
        TxData data;
    };
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

struct PACKED TxIo
{
    uint16_t port;
    uint32_t data;
    int size;
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

SYSCALL(exit)
(uint64_t exit_code);

SYSCALL(exec)
(const char *name, uint64_t param);

SYSCALL(in)
(TxIo *io);

SYSCALL(out)
(TxIo *io);

SYSCALL(alloc)
(uint64_t pages, void **ptr);
#define FOREACH_SYSCALLS(SYSCALL_) \
    SYSCALL_(DEBUG)                \
    SYSCALL_(IPC)                  \
    SYSCALL_(BIND)                 \
    SYSCALL_(MAP)                  \
    SYSCALL_(EXIT)                 \
    SYSCALL_(EXEC)                 \
    SYSCALL_(ALLOC)                \
    SYSCALL_(IN)                   \
    SYSCALL_(OUT)

typedef enum
{
#define ITER(SYSCALL_) TX_SYS_##SYSCALL_,
    FOREACH_SYSCALLS(ITER)

#undef ITER

        TX_SYS_COUNT
} TxSyscall;

enum TxErrors
{
    TX_SUCCESS,
    TX_BAD_CAPABILITY,
    TX_INVALID_PARAMS,
    TX_NOT_FOUND,
    TX_NOT_ALLOWED,
};

extern "C" TxResult
tx_syscall(TxSyscall syscall, uint64_t arg0, uint64_t arg1 = 0, uint64_t arg2 = 0, uint64_t arg3 = 0, uint64_t arg4 = 0);