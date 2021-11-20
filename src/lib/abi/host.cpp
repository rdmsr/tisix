#include "abi/syscalls.hpp"
#include "tisix/arch.hpp"
#include "tisix/stream.hpp"
#include <syscalls.hpp>

class SyscallWriter : public tisix::Stream<const char *>
{
public:
    void write(const char *s) override
    {
        tx_sys_debug((char *)s);
    }

    void putc(char c) override
    {
        char s[] = {c, 0};

        tx_sys_debug(s);
    }
};

// Arch *get_arch()
//{
//     static Arch arch;
//     static SyscallWriter writer;
//
//     arch.debug_stream = nullptr;
//
//     return &arch;
// }
