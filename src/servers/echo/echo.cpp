#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

extern "C" void _start(tisix::Handover *handover)
{

    log("hello from the echo server");

    log("handover = {#p}", (uint64_t)handover);

    tx_sys_exit(TX_SUCCESS);
}