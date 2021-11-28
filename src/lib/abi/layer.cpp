#include <abi/layer.hpp>

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