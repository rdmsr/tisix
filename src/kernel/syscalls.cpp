#include "abi/syscalls.hpp"
#include "event.hpp"
#include "loader.hpp"
#include "tisix/handover.hpp"
#include "tisix/host.hpp"
#include "vmm.hpp"
#include <asm.hpp>
#include <gdt.hpp>
#include <ipc.hpp>
#include <loader.hpp>
#include <syscalls.hpp>
#include <tisix/arch.hpp>
#include <tisix/assert.hpp>
#include <tisix/std.hpp>

using namespace tisix;
void syscall_set_gs(uintptr_t addr)
{
    asm_write_msr(MSR_GS_BASE, addr);
    asm_write_msr(MSR_KERN_GS_BASE, addr);
}

TxResult sys_debug(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{
    (void)args2;
    (void)args3;
    (void)args4;

    auto unpacked = (char *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    host_log_write(unpacked);

    return TX_SUCCESS;
}

TxResult sys_ipc(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{
    (void)args2;
    (void)args3;
    (void)args4;

    auto unpacked = (TxIpc *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    if ((unpacked->flags & TX_IPC_SEND) && unpacked->to != get_sched()->current_task->id)
    {
        ipc_send(unpacked);
    }

    else if (get_sched()->current_task->id != get_sched()->current_task->ipc_buffer->msg.from)
    {
        ipc_recv(unpacked);
    }

    return TX_SUCCESS;
}

TxResult sys_bind(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{
    (void)args2;
    (void)args3;
    (void)args4;

    auto unpacked = (TxEvent *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    tisix::bind_event(get_sched()->current_task, *unpacked);

    return TX_SUCCESS;
}

static uint32_t lock = 0;

TxResult sys_map(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{
    (void)args2;
    (void)args3;
    (void)args4;

    lock_acquire(&lock);

    auto unpacked = (TxMap *)args;

    if (!unpacked)
    {
        return TX_INVALID_PARAMS;
    }

    for (size_t i = 0; i < ALIGN_UP(unpacked->size, 4096) / 4096; i++)
    {

        host_map_memory(get_sched()->current_task->pagemap, i * PAGE_SIZE + unpacked->phys, i * PAGE_SIZE + unpacked->virt, unpacked->flags);
    }

    lock_release(&lock);

    return TX_SUCCESS;
}

TxResult sys_exit(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{
    (void)args2;
    (void)args3;
    (void)args4;

    get_sched()->current_task->running = false;
    get_sched()->current_task->return_value = args;

    while (1)
    {
        asm_sti();
    }
    return TX_SUCCESS;
}

TxResult sys_exec(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{

    (void)args;
    (void)args2;
    (void)args3;
    (void)args4;

    auto name = (const char *)args;

    loader_new_elf_task(name, TX_TASK_USER, 0);

    return TX_SUCCESS;
}

TxResult sys_alloc(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{
    (void)args3;
    (void)args4;

    auto unpacked = (void **)args2;

    *unpacked = host_allocate_pages(args);

    return TX_SUCCESS;
}

TxResult sys_free(uint64_t args, uint64_t args2, uint64_t args3, uint64_t args4)
{
    (void)args3;
    (void)args4;

    host_free_pages((void *)args, args2);

    return TX_SUCCESS;
}

TxResult sys_in(uint64_t arg1, uint64_t, uint64_t, uint64_t)
{
    if (!(get_sched()->current_task->capabilities & TX_CAP_IO))
    {
        return TX_BAD_CAPABILITY;
    }

    auto unpacked = (TxIo *)arg1;

    unpacked->data = arch_in(unpacked->port, unpacked->size);

    return TX_SUCCESS;
}

TxResult sys_out(uint64_t arg1, uint64_t, uint64_t, uint64_t)
{
    if (!(get_sched()->current_task->capabilities & TX_CAP_IO))
    {
        return TX_BAD_CAPABILITY;
    }

    auto unpacked = (TxIo *)arg1;

    arch_out(unpacked->port, unpacked->data, unpacked->size);

    return TX_SUCCESS;
}

typedef TxResult TxSyscallFn(uint64_t a, uint64_t b, uint64_t c, uint64_t d);

static TxSyscallFn *syscalls[TX_SYS_COUNT] = {
    [TX_SYS_DEBUG] = sys_debug,
    [TX_SYS_IPC] = sys_ipc,
    [TX_SYS_BIND] = sys_bind,
    [TX_SYS_MAP] = sys_map,
    [TX_SYS_EXIT] = sys_exit,
    [TX_SYS_EXEC] = sys_exec,
    [TX_SYS_ALLOC] = sys_alloc,
    [TX_SYS_FREE] = sys_free,
    [TX_SYS_IN] = sys_in,
    [TX_SYS_OUT] = sys_out};

extern "C" void syscall_handler(Stack *stack)
{
    syscall_dispatch(stack, (TxSyscall)stack->rax, stack->rbx);
}

TxResult syscall_dispatch(Stack *stack, TxSyscall sys_number, uint64_t args)
{
    auto result = syscalls[sys_number](args, stack->rcx, stack->rdx, stack->rsi);

    stack->rax = result;

    return result;
}

extern "C" void syscall_handle(void);

void syscall_init(void)
{
    asm_write_msr(MSR_EFER, asm_read_msr(MSR_EFER) | 1);
    asm_write_msr(MSR_STAR, ((uint64_t)(GDT_KERNEL_CODE * 8) << 32) | ((uint64_t)(((GDT_USER_DATA - 1) * 8) | 3) << 48));
    asm_write_msr(MSR_LSTAR, (uint64_t)syscall_handle);
    asm_write_msr(MSR_SYSCALL_FLAG_MASK, 0);
}
