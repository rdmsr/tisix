#include "abi/syscalls.hpp"
#include <ipc.hpp>
#include <loader.hpp>
#include <tisix/mem.hpp>

using namespace tisix;

void tisix::ipc_send(TxIpc *args)
{
    for (auto i : get_sched()->tasks)
    {
        if (i->id == args->to)
        {
            tisix::memcpy(i->ipc_buffer, args, sizeof(TxIpc));

            i->ipc_buffer->received = true;

            i->ipc_buffer->msg.from = get_sched()->current_task->id;

            break;
        }

        if (i == *get_sched()->tasks.end())
        {
            panic("ipc error: couldn't find task with id {}", args->to);
        }
    }
}

void tisix::ipc_recv(TxIpc *buffer)
{
    tisix::memcpy(buffer, get_sched()->current_task->ipc_buffer, sizeof(TxIpc));

    get_sched()->current_task->ipc_buffer->received = false;

    get_sched()->current_task->ipc_buffer->msg.type = TX_MSG_NONE;
}