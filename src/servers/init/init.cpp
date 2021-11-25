#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

extern "C" void _start(tisix::Handover *handover)
{

    (void)handover;
    log("booting up other servers...");

    tx_sys_exec("fb", (uint64_t)handover);
    tx_sys_exec("echo", (uint64_t)0);

    log("ok");
    tx_sys_exit(TX_SUCCESS);
}