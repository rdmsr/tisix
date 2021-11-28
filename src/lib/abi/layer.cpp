#include "abi/syscalls.hpp"
#include <abi/layer.hpp>
#include <tisix/assert.hpp>

void tisix::ipc_on_receive(bool (*f)(TxIpc ipc))
{
    TxIpc ipc = {};
    ipc.flags = TX_IPC_RECV;

    while (tx_sys_ipc(&ipc) == 0)
    {
        if (ipc.msg.type != TX_MSG_NONE && ipc.received)
        {
            if (f(ipc))
                break;
        }
    }
}

void tisix::ipc_send(size_t pid, uint64_t data, TxMsgType type)
{
    TxIpc ipc = {};

    ipc.to = pid;
    ipc.msg.type = type;

    ipc.msg.data = data;

    tx_sys_ipc(&ipc);
}

void tisix::ipc_send(size_t pid, TxEvent data)
{
    TxIpc ipc = {};

    ipc.to = pid;

    ipc.msg.type = TX_MSG_EVENT;

    ipc.msg.event = data;

    tx_sys_ipc(&ipc);
}

uint32_t tisix::in(uint16_t port, int size)
{
    TxIo io;
    io.size = size;
    io.port = port;

    assert(tx_sys_in(&io) == TX_SUCCESS);

    return io.data;
}

void tisix::out(uint16_t port, uint32_t data, int size)
{
    TxIo io;
    io.size = size;
    io.port = port;
    io.data = data;

    assert(tx_sys_out(&io) == TX_SUCCESS);
}
