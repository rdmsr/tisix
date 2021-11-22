#include "abi/syscalls.hpp"
#include <syscalls.hpp>

extern "C" TxResult tx_sys_debug(const char *args)
{
    return tx_syscall(TX_SYS_DEBUG, (uint64_t)args);
}

extern "C" TxResult tx_sys_ipc(TxIpc *ipc)
{
    return tx_syscall(TX_SYS_IPC, (uint64_t)ipc);
}

extern "C" TxResult tx_sys_bind(TxEvent *event)
{
    return tx_syscall(TX_SYS_BIND, (uint64_t)event);
}

extern "C" TxResult tx_sys_map(TxMap *map)
{
    return tx_syscall(TX_SYS_MAP, (uint64_t)map);
}

extern "C" TxResult tx_syscall(TxSyscall syscall, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
{
    uint64_t res = 0;
#ifdef __x86_64__

    register uint64_t r8 asm("r8") = arg3;
    register uint64_t r9 asm("r9") = arg4;

    asm volatile("int $0x42"
                 : "=a"(res)
                 : "a"(syscall), "b"(arg0), "c"(arg1), "d"(arg2), "r"(r8), "r"(r9)
                 : "memory");
#endif

    return res;
}