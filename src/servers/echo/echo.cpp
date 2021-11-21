#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

extern "C" void _start()
{
    TxIpc ipc;

    ipc.flags = TX_IPC_RECV;

    TxEvent irq0 = {.type = TX_EVENT_IRQ, .irq = 0};

    tx_sys_bind(&irq0);

    while (tx_sys_ipc(&ipc) == 0)
    {
        if (ipc.msg.event.type == TX_EVENT_IRQ)
        {
            log("got timer interrupt");
        }
    }

    while (1)
        ;
}