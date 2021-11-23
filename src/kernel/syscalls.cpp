#include "abi/syscalls.hpp"
#include "event.hpp"
#include "loader.hpp"
#include "tisix/handover.hpp"
#include "tisix/host.hpp"
#include "vmm.hpp"
#include <asm.hpp>
#include <ipc.hpp>
#include <loader.hpp>
#include <syscalls.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>
#include <tisix/std.hpp>

using namespace tisix;

TxResult sys_debug(uint64_t args)
{
    auto unpacked = (char *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    host_log_write(unpacked);

    return TX_SUCCESS;
}

TxResult sys_ipc(uint64_t args)
{
    auto unpacked = (TxIpc *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    if ((unpacked->flags & TX_IPC_SEND) && unpacked->to != get_sched()->current_task->id)
    {
        ipc_send(unpacked);
    }

    else if (get_sched()->current_task->id != get_sched()->current_task->ipc_buffer->msg.from)
    {
        ipc_recv(unpacked);
    }

    return TX_SUCCESS;
}

TxResult sys_bind(uint64_t args)
{
    auto unpacked = (TxEvent *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    tisix::bind_event(get_sched()->current_task, *unpacked);

    return TX_SUCCESS;
}

TxResult sys_map(uint64_t args)
{

    auto unpacked = (TxMap *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    host_map_memory(get_sched()->current_task->pagemap, unpacked->phys, unpacked->virt, unpacked->flags);

    return TX_SUCCESS;
}

TxResult sys_exit(uint64_t args)
{

    get_sched()->current_task->running = false;
    get_sched()->current_task->return_value = args;

    asm_sti();

    while (1)
        ;

    return 0;
}

typedef TxResult TxSyscallFn(uint64_t);

static TxSyscallFn *syscalls[TX_SYS_COUNT] = {
    [TX_SYS_DEBUG] = sys_debug,
    [TX_SYS_IPC] = sys_ipc,
    [TX_SYS_BIND] = sys_bind,
    [TX_SYS_MAP] = sys_map,
    [TX_SYS_EXIT] = sys_exit,
};

TxResult syscall_dispatch(Stack *stack, TxSyscall sys_number, uint64_t args)
{
    auto result = syscalls[sys_number](args);

    stack->rax = result;

    return result;
}