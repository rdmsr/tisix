#pragma once
#include <tisix/log.hpp>
#include <tisix/std.hpp>

namespace tisix
{
inline int _debug_counter = 0;

#define debug() \
    log("{}", _debug_counter++);

} // namespace tisix