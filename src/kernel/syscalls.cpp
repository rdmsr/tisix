#include "abi/syscalls.hpp"
#include "event.hpp"
#include "loader.hpp"
#include "tisix/fmt.hpp"
#include "tisix/handover.hpp"
#include "tisix/host.hpp"
#include "tisix/maybe.hpp"
#include "tisix/mem.hpp"
#include "vmm.hpp"
#include <ipc.hpp>
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

    if ((unpacked->flags & TX_IPC_SEND) && unpacked->to != get_sched()->current_task->id)
    {

        ipc_send(unpacked);

        return 0;
    }

    else if (get_sched()->current_task->id != get_sched()->current_task->ipc_buffer->msg.from)
    {

        ipc_recv(unpacked);

        return 0;
    }

    return 0;
}

TxResult sys_bind(void *args)
{
    auto unpacked = (TxEvent *)args;

    tisix::bind_event(get_sched()->current_task, *unpacked);

    return 0;
}

TxResult sys_map(void *args)
{
    auto unpacked = (TxMap *)args;

    host_map_memory(get_sched()->current_task->pagemap, unpacked->phys, unpacked->virt, unpacked->flags);

    return 0;
}

typedef TxResult TxSyscallFn(void *);

static TxSyscallFn *syscalls[TX_SYS_COUNT] = {
    [TX_SYS_DEBUG] = sys_debug,
    [TX_SYS_IPC] = sys_ipc,
    [TX_SYS_BIND] = sys_bind,
    [TX_SYS_MAP] = sys_map,

};

TxResult syscall_dispatch(TxSyscall sys_number, uint64_t args)
{
    auto result = syscalls[sys_number]((void *)args);

    assert(result == 0);
    return result;
}