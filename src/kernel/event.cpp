#include "ipc.hpp"
#include "tisix/list.hpp"
#include <event.hpp>
#include <tisix/vec.hpp>

using namespace tisix;
static Vec<EventBinding> bindings;

void tisix::events_init()
{
    bindings.construct();
}

static Maybe<EventBinding> get_binding(Task *task, TxEvent event)
{
    for (auto i : bindings)
    {
        if (i.task == task && i.event.type == event.type && i.event.irq == event.irq)
        {
            return Just(i);
        }
    }
    return Nothing;
}

void tisix::bind_event(Task *task, TxEvent event)
{
    auto binding = get_binding(task, event);

    if (binding.success)
    {
        return;
    }

    EventBinding new_binding = {.task = task, .event = event};

    bindings.push(new_binding);
}

void tisix::trigger_event(TxEvent event)
{
    for (auto i : bindings)
    {
        if (i.event.type == event.type && i.event.irq == event.irq)
        {
            TxIpc msg = {};

            msg.to = i.task->id;
            msg.flags = TX_IPC_SEND;
            msg.msg = {.from = TX_FROM_EVENT, .event = event};

            ipc_send(&msg);
        }
    }
}
