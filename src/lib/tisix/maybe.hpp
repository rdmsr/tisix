#pragma once
#include "tisix/string_view.hpp"
#include <tisix/log.hpp>
#include <tisix/std.hpp>

namespace tisix
{
template <typename T>
class Maybe
{
public:
    bool success;
    T value;

    T value_or(T val)
    {
        return this->success ? value : val;
    }

    template <typename function>
    T value_or(function f)
    {
        if (this->success)
            return value;
        else
            f();

        return (T){};
    }

    T value_or_error(StringView message)
    {
        if (this->success)
            return value;

        log_error("{}", message);

        return;
    }
};

#define Just(val) \
    {true, val};

#define error_to_lambda(msg, ...) \
    [&]() {                       \
        log_error("{}", msg);     \
        __VA_ARGS__               \
    }

#define panic_to_lambda(msg, ...) \
    [&]() {                       \
        panic("{}", msg);         \
        __VA_ARGS__               \
    }
#define Nothing \
    {false, {}};

} // namespace tisix