#include "abi/layer.hpp"
#include "tasking.hpp"
#include "tisix/alloc.hpp"
#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

struct Time
{
    int hour = 0;
    int minute = 0;
    int second = 0;
};

int main(void)
{

    TxIpc ipc = {};

    ipc.to = 2;
    ipc.msg.type = TX_MSG_REQUEST;

    tx_sys_ipc(&ipc);

    tisix::ipc_on_receive([](TxIpc ipc)
                          {

                        if(ipc.msg.type == TX_MSG_RESPONSE_DATA)
                        {
                            Time* time = (Time*)ipc.msg.data;

                            log("Got time from time server: {}:{}:{}",time->hour, time->minute, time->second);

                            return true;
                        }

                        return false; });
    return 0;
}
