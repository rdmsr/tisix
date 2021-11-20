#pragma once
#include <tasking.hpp>

namespace tisix
{
class Scheduler
{
public:
    Scheduler(size_t time_slice);
    Scheduler() : time_slice(10) {}
    void construct(size_t time_slice);

    tisix::Maybe<Task *> tick();
    void add_task(Task *t);

    bool ready() { return _ready; }

    tisix::Vec<Task *> tasks;

    Task *current_task;

    bool _ready = false;

private:
    size_t index = 0;
    size_t counter = 0;
    size_t time_slice = 1;
};
} // namespace tisix