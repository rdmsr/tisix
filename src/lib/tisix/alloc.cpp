#include <tisix/alloc.hpp>

using namespace tisix;

void *tisix::malloc(size_t size)
{
    return ((Alloc *)host_get_alloc(size))->allocate(size).value_or(panic_to_lambda("moment"));
}

void tisix::free(void *ptr)
{
    ((Alloc *)host_get_alloc())->free(ptr);
}