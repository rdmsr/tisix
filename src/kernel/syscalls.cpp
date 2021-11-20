#include "abi/syscalls.hpp"
#include "loader.hpp"
#include "tisix/fmt.hpp"
#include <loader.hpp>
#include <syscalls.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>
#include <tisix/std.hpp>

TxResult sys_debug(void *args)
{
    auto unpacked = (char *)args;

    assert(args != nullptr);

    tisix::fmt_stream(get_arch()->debug_stream, "{}: ", tisix::get_sched()->current_task->name);

    get_arch()->debug_stream->write(unpacked);

    return 0;
}

typedef TxResult TxSyscallFn(void *);

TxSyscallFn *syscalls[TX_SYS_COUNT] = {
    [TX_SYS_DEBUG] = sys_debug,
};

TxResult syscall_dispatch(TxSyscall sys_number, uint64_t args)
{
    return syscalls[sys_number]((void *)args);
}