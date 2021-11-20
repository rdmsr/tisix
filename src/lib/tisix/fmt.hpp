#pragma once
#include <stdarg.h>
#include <tisix/stream.hpp>
#include <tisix/string_view.hpp>

namespace tisix
{

enum FmtType
{
    FMT_CHAR,
    FMT_STRING,
    FMT_BOOL,
    FMT_SIGNED,
    FMT_UNSIGNED,
    FMT_POINTER

};

struct FmtValue
{
    enum FmtType type;

    union
    {
        char _char;
        StringView string;
        bool _bool;
        long _signed;
        unsigned long long _unsigned;
        void *ptr;
    };
};

struct FmtArgs
{
    size_t count;
    FmtValue *values;
};

static inline FmtValue fmt_match(char c)
{
    return (FmtValue){.type = FMT_CHAR, ._char = c};
}

static inline FmtValue fmt_match(StringView s)
{
    return (FmtValue){.type = FMT_STRING, .string = s};
}

static inline FmtValue fmt_match(const char *s)
{
    return (FmtValue){.type = FMT_STRING, .string = s};
}

static inline FmtValue fmt_match(char *s)
{
    return (FmtValue){.type = FMT_STRING, .string = s};
}

static inline FmtValue fmt_match(bool b)
{
    return (FmtValue){.type = FMT_BOOL, ._bool = b};
}

static inline FmtValue fmt_match(void *ptr)
{
    return (FmtValue){.type = FMT_POINTER, .ptr = ptr};
}
static inline FmtValue fmt_match(unsigned char c)
{
    return (FmtValue){.type = FMT_UNSIGNED, ._unsigned = c};
}

#define make_fmt_match_number(T)                                 \
    static inline FmtValue fmt_match(T n)                        \
    {                                                            \
        return (FmtValue){.type = FMT_SIGNED, ._signed = n};     \
    }                                                            \
    static inline FmtValue fmt_match(unsigned T n)               \
    {                                                            \
        return (FmtValue){.type = FMT_UNSIGNED, ._unsigned = n}; \
    }

make_fmt_match_number(short);
make_fmt_match_number(long);
make_fmt_match_number(int);
make_fmt_match_number(long long);

template <typename T>
static inline FmtValue fmt_match(T class_)
{
    if (__is_class(T))
        return (FmtValue){.type = FMT_STRING, .string = class_.show()};

    return (FmtValue){.type = FMT_STRING, .string = "non-printable"};
}
template <size_t N>
struct FmtStorage
{
    FmtValue values[N];
    FmtArgs args{N, values};
};

template <typename... T>
auto fmt_make_storage(T... t)
{
    return FmtStorage<sizeof...(T)>{.values = {fmt_match(t)...}};
}

void fmt_stream_impl(void (*callback)(const char *s), StringView fmt, FmtArgs args);

#define fmt_stream(stream, fmt, ...) \
    fmt_stream_impl(stream, fmt, tisix::fmt_make_storage(__VA_ARGS__).args);

} // namespace tisix