#pragma once

#include <tisix/std.hpp>

namespace tisix
{

// Stream as seen in the C++ std library
template <class T>
class Stream
{
public:
    virtual void write(T text) { (void)text; };
    virtual T read() { return 0; };
    virtual void putc(char c) { (void)c; };
};

template class Stream<char const *>;
} // namespace tisix