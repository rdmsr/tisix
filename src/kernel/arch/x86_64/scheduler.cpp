#include "tisix/maybe.hpp"
#include "tisix/std.hpp"
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
    lock_acquire(&lock);

    tasks.push(t);

    if (tasks.size == 1)
        current_task = tasks[0];
    lock_release(&lock);
}

Maybe<Task *> Scheduler::tick()
{
    lock_acquire(&lock);

    if (counter++ == time_slice)
    {
        counter = 0;

        if (tasks[index])
        {
            while (tasks[index]->running == false)
            {
                if (index + 1 == tasks.size)
                    index = 0;
                else
                    index++;
            }
        }
    }

    lock_release(&lock);

    if (tasks[index])
    {
        return Just(tasks[index]);
    }
    else
        return Nothing;
}
