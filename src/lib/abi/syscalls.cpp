#include "abi/syscalls.hpp"
#include <syscalls.hpp>
extern "C" TxResult tx_sys_debug(const char *args)
{
    return tx_syscall(TX_SYS_DEBUG, (void *)args);
}

extern "C" TxResult tx_sys_ipc(TxIpc *ipc)
{
    return tx_syscall(TX_SYS_IPC, (void *)ipc);
}

extern "C" TxResult tx_sys_bind(TxEvent *event)
{
    return tx_syscall(TX_SYS_BIND, (void *)event);
}

extern "C" TxResult tx_sys_map(TxMap *map)
{
    return tx_syscall(TX_SYS_MAP, (void *)map);
}

extern "C" TxResult tx_syscall(TxSyscall syscall, void *arg)
{
    // FIXME: get result of syscall
    uint64_t res = 0;
#ifdef __x86_64__
    asm volatile("int $0x42" ::"b"((int)syscall), "c"(arg));
#endif

    return res;
}