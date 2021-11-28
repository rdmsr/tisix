#include "abi/syscalls.hpp"
#include <syscalls.hpp>

extern "C"
{
    TxResult tx_sys_debug(const char *args)
    {
        return tx_syscall(TX_SYS_DEBUG, (uint64_t)args);
    }

    TxResult tx_sys_ipc(TxIpc *ipc)
    {
        return tx_syscall(TX_SYS_IPC, (uint64_t)ipc);
    }

    TxResult tx_sys_bind(TxEvent *event)
    {
        return tx_syscall(TX_SYS_BIND, (uint64_t)event);
    }

    TxResult tx_sys_map(TxMap *map)
    {
        return tx_syscall(TX_SYS_MAP, (uint64_t)map);
    }

    TxResult tx_sys_exit(uint64_t args)
    {
        return tx_syscall(TX_SYS_EXIT, args);
    }

    TxResult tx_sys_exec(const char *name, uint64_t param)
    {
        return tx_syscall(TX_SYS_EXEC, (uint64_t)name, param);
    }

    TxResult tx_sys_alloc(uint64_t pages, void **ptr)
    {
        return tx_syscall(TX_SYS_ALLOC, pages, (uint64_t)ptr);
    }

    TxResult tx_sys_free(void *ptr, uint64_t pages)
    {
        return tx_syscall(TX_SYS_FREE, (uint64_t)ptr, pages);
    }

    TxResult tx_sys_in(TxIo *io)
    {
        return tx_syscall(TX_SYS_IN, (uint64_t)io);
    }

    TxResult tx_sys_out(TxIo *io)
    {
        return tx_syscall(TX_SYS_OUT, (uint64_t)io);
    }

    TxResult tx_syscall(TxSyscall syscall, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
    {
        uint64_t res = 0;
#ifdef __x86_64__

        asm volatile("int $0x42"
                     : "=a"(res)
                     : "a"(syscall), "b"(arg0), "c"(arg1), "d"(arg2), "S"(arg3), "D"(arg4)
                     : "memory");
#endif

        return res;
    }
}