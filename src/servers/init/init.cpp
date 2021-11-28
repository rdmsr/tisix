#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

int main(void)
{
    log("hello from init");

    TxIpc ipc = {};

    ipc.msg.type = TX_MSG_DATA;
    ipc.msg.data = 10;

    ipc.to = 2;

    tx_sys_ipc(&ipc);

    return 0;
}