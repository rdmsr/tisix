#include "abi/syscalls.hpp"
#include <abi/layer.hpp>
#include <tisix/assert.hpp>

void tisix::ipc_on_receive(bool (*f)(TxIpc ipc))
{
    TxIpc ipc = {};
    ipc.flags = TX_IPC_RECV;

    while (tx_sys_ipc(&ipc) == 0)
    {
        if (ipc.msg.type != TX_MSG_NONE)
        {
            if (f(ipc))
                break;
        }
    }
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
