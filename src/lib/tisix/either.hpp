#pragma once

namespace tisix
{
template <typename T, typename B>
struct Either
{

    T left;
    B right;

    bool success = false;
};

#define Left(v)         \
    {                   \
        .left = v,      \
        .right = {},    \
        .success = true \
    }

#define Right(v)         \
    {                    \
        .right = v,      \
        .left = {},      \
        .success = false \
    }
} // namespace tisix