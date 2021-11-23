#include "tisix/maybe.hpp"
#include <scheduler.hpp>
#include <tisix/assert.hpp>
#include <tisix/debug.hpp>

using namespace tisix;

static uint32_t lock = 0;

Scheduler::Scheduler(size_t m_time_slice)
{
    time_slice = m_time_slice;
    _ready = true;
}

void Scheduler::construct(size_t m_time_slice)
{
    time_slice = m_time_slice;

    tasks.construct();

    counter = time_slice - 1;

    index = 0;

    _ready = true;
}

void Scheduler::add_task(Task *t)
{
    tasks.push(t);

    if (tasks.size == 1)
        current_task = tasks[0];
}

Maybe<Task *> Scheduler::tick()
{
    lock_acquire(&lock);

    counter++;

    if (counter > time_slice)
        counter = 0;

    if (counter == time_slice && tasks.size > 0)
    {
        int prev_index = index;

        if (index + 1 < tasks.size)
        {
            index++;
        }

        // if we're at the end of the list, rewind
        else if (index + 1 == tasks.size)
        {
            index = 0;
        }

        counter = 0;

        lock_release(&lock);

        if (tasks[prev_index]->running)
            return Just(tasks[prev_index]);
        return Nothing;
    }

    lock_release(&lock);
    return Nothing;
}
