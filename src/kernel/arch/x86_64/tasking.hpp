#pragma once
#include <abi/syscalls.hpp>
#include <common.hpp>
#include <tisix/string_view.hpp>
#include <tisix/vec.hpp>

#include <vmm.hpp>

#define TX_NONE (uint8_t)(0)
#define TX_USER (uint8_t)(1 << 0)

struct Task
{
    Stack stack;

    TxIpc *ipc_buffer;

    uintptr_t sp;
    uint64_t *pagemap;

    uint8_t flags;

    uint64_t id = 0;
    uint64_t return_value = 0;
    uint64_t index = 0;

    bool running = true;

    tisix::StringView name;

    Task(tisix::StringView name, uint8_t flags);

    void start(uintptr_t ip);

    Task(){};
};
