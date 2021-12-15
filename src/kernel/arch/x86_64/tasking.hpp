#pragma once
#include <abi/syscalls.hpp>
#include <common.hpp>
#include <tisix/list.hpp>
#include <tisix/string_view.hpp>

#include <vmm.hpp>

struct Task
{
    Stack stack;

    TxIpc *ipc_buffer;

    uintptr_t sp;
    uint64_t *pagemap;

    uint8_t flags;
    uint8_t capabilities;

    uint64_t id = 0;
    uint64_t return_value = 0;
    uint64_t index = 0;

    bool running = true;

    tisix::StringView name;

    Task(tisix::StringView name, uint8_t flags, uint8_t caps);

    void start(uintptr_t ip);

    Task(){};
};
