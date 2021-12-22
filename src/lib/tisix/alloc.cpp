#include <tisix/alloc.hpp>

#ifdef HOST
#    include <cstdlib>
#endif

using namespace tisix;

void *tisix::malloc(size_t size)
{
#ifndef HOST
    return ((Alloc *)host_get_alloc(size))->allocate(size).value_or(panic_to_lambda("moment"));
#else
    return ::malloc(size);
#endif
}

void tisix::free(void *ptr)
{
#ifndef HOST
    ((Alloc *)host_get_alloc())->free(ptr);
#else
    return ::free(ptr);
#endif
}
