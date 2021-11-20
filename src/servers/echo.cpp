#include <abi/syscalls.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

extern "C" void _start()
{

    const char *albums[] = {
        "Wish you were here",
        "Dark side of the moon",
        "Animals",
        "The Wall",
        "Meddle"};

    log("Best Pink floyd albums:");
    for (size_t i = 1; i < sizeof(albums) / sizeof(*albums); i++)
    {
        log("\t{}. {}", i, albums[i - 1]);
    }

    while (1)
        ;
}