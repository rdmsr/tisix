#pragma once

#include <tisix/std.hpp>

namespace tisix
{
class StringView
{
public:
    const char *buffer;
    size_t size;

public:
    StringView(const char *str);
    StringView();

    bool operator==(const char *s);
    bool operator==(StringView v);
    char operator[](size_t index);

    const char *begin();
    const char *end();
};

StringView to_str(const char *str);

char *itoa(int64_t value, char *str, uint16_t base);

} // namespace tisix