#include "abi/layer.hpp"
#include <tisix/alloc.hpp>
#include <tisix/log.hpp>

struct Time
{
    int hour = 0;
    int minute = 0;
    int second = 0;
};

#define time_pad(val) (val < 10) ? '0' : '\0'

int main(void)
{
    // tisix::ipc_send(2, 0, TX_MSG_REQUEST);
    //
    // tisix::ipc_on_receive([](TxIpc ipc)
    //                      {
    //                        Time *time = (Time *)ipc.msg.data;
    //
    //                        log("Got time from pid #{}: {}{}:{}{}:{}{}", ipc.msg.from, time_pad(time->hour), time->hour, time_pad(time->minute), time->minute, time_pad(time->second), time->second);
    //
    //                        return true; });
    return 0;
}
