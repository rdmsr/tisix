#include "abi/syscalls.hpp"
#include "loader.hpp"
#include "tisix/fmt.hpp"
#include <loader.hpp>
#include <syscalls.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>
#include <tisix/std.hpp>

using namespace tisix;

TxResult sys_debug(void *args)
{
    auto unpacked = (char *)args;

    assert(args != nullptr);

    host_log_write(unpacked);

    return 0;
}

typedef TxResult TxSyscallFn(void *);

static TxSyscallFn *syscalls[TX_SYS_COUNT] = {
    [TX_SYS_DEBUG] = sys_debug,
};

TxResult syscall_dispatch(TxSyscall sys_number, uint64_t args)
{
    return syscalls[sys_number]((void *)args);
}