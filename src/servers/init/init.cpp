#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

int main(void)
{

    TxIpc ipc = {};

    ipc.msg.type = TX_MSG_DATA;
    ipc.msg.data = 10;

    ipc.to = 3;

    tx_sys_ipc(&ipc);

    return 0;
}