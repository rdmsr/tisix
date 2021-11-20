#include <abi/syscalls.hpp>
#include <tisix/log.hpp>
#include <tisix/std.hpp>

extern "C" void _start()
{

    log("hello from the fb server");

    TxIpc ipc;

    ipc.to = 0;
    ipc.send = true;
    ipc.number = 10;

    tx_sys_ipc(&ipc);

    while (1)
        ;
}