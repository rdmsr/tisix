#pragma once
#include <abi/syscalls.hpp>
#include <tasking.hpp>

namespace tisix
{

struct EventBinding
{
    Task *task;
    TxEvent event;
};

void bind_event(Task *task, TxEvent event);
void unbind_event(Task *task, TxEvent event);
void trigger_event(TxEvent event);
void events_init();

} // namespace tisix