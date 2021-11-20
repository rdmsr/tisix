#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

extern "C" void _start()
{
    TxIpc ipc;
    ipc.send = false;

    while (1)
    {
        tx_sys_ipc(&ipc);

        if (ipc.received == true)
        {
            log("Received {} from task #{}", ipc.number, ipc.from);
            break;
        }
    }

    while (1)
        ;
}