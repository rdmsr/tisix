#include <abi/syscalls.hpp>

extern "C" void _start()
{
    tx_sys_debug("Hello from the echo server");
    while (1)
        ;
}