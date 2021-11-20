#pragma once
#include <tisix/arch.hpp>

namespace tisix
{
#define assert_unreachable() panic("unreachable");
#define assert(b)     \
    if ((b) == false) \
        panic("assertion {} failed", #b);
} // namespace tisix