#include "tisix/handover.hpp"
#include <abi/syscalls.hpp>

#define WEAK __attribute__((weak))

WEAK int main(int argc, char const **argv)
{
    (void)argc;
    (void)argv;

    return 0;
}

WEAK int handover_main(tisix::Handover *handover)
{
    (void)handover;
    return 0;
}

extern "C" void _start(TxEntryType type, long arg1, long arg2, long arg3)
{
    int ex = 0;

    (void)arg3;
    switch (type)
    {
    case TX_ENTRY_CMAIN:
    {
        ex = main((int)arg1, (const char **)arg2);
        break;
    }

    case TX_ENTRY_HANDOVER:
    {
        ex = handover_main((tisix::Handover *)arg1);
        break;
    }
    }

    tx_sys_exit(ex);

    __builtin_unreachable();
}
