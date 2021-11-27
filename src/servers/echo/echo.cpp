#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/alloc.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

extern "C" void _start(void)
{
    log("Hello, world!");

    tx_sys_exit(TX_SUCCESS);
}