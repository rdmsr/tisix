#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

extern "C" void _start(tisix::Handover *handover)
{

    (void)handover;

    tx_sys_exit(TX_SUCCESS);
}