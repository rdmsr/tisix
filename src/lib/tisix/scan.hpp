#pragma once
#include <tisix/std.hpp>
#include <tisix/string_view.hpp>

namespace tisix
{
class Scanner
{
public:
    size_t head, size;
    const char *buffer;

public:
    char forward();
    char forward(size_t n);

    bool ended();

    char current();
    char peek(size_t n);

    Scanner(StringView s);
};
} // namespace tisix