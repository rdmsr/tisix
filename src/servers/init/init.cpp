#include "abi/layer.hpp"
#include <abi/syscalls.hpp>
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
    log("Welcome to the machine!");

    log("Exiting.");

    return 0;
}
