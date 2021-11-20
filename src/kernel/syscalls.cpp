#include "abi/syscalls.hpp"
#include "loader.hpp"
#include "tisix/fmt.hpp"
#include "tisix/mem.hpp"
#include <loader.hpp>
#include <syscalls.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>
#include <tisix/std.hpp>

using namespace tisix;

TxResult sys_debug(void *args)
{
    auto unpacked = (char *)args;

    assert(args != nullptr);

    host_log_write(unpacked);

    return 0;
}

TxResult sys_ipc(void *args)
{
    auto unpacked = (TxIpc *)args;

    if (unpacked->send == true && unpacked->to != get_sched()->current_task->id)
    {
        for (auto i : get_sched()->tasks)
        {
            if (i->id == unpacked->to)
            {
                tisix::memcpy(i->ipc_buffer, unpacked, sizeof(TxIpc));

                i->ipc_buffer->from = get_sched()->current_task->id;

                unpacked->received = true;

                break;
                return 0;
            }

            if (i == *get_sched()->tasks.end())
                panic("ipc error: couldn't find task with id {}", unpacked->to);
        }
    }

    else if (get_sched()->current_task->id != get_sched()->current_task->ipc_buffer->from)
    {
        tisix::memcpy(unpacked, get_sched()->current_task->ipc_buffer, sizeof(TxIpc));

        unpacked->received = true;

        return 0;
    }

    return -1;
}

typedef TxResult TxSyscallFn(void *);

static TxSyscallFn *syscalls[TX_SYS_COUNT] = {
    [TX_SYS_DEBUG] = sys_debug,
    [TX_SYS_IPC] = sys_ipc,
};

TxResult syscall_dispatch(TxSyscall sys_number, uint64_t args)
{
    return syscalls[sys_number]((void *)args);
}