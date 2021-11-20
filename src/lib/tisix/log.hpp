#pragma once
#include <tisix/fmt.hpp>

namespace tisix
{
enum LogType
{
    LOG_INFO,
    LOG_ERROR,
    LOG_WARNING
};

void log_impl(const char *file, int line, StringView fmt, FmtArgs args, LogType type = LOG_INFO);

#define log(fmt, ...) log_impl(__FILENAME__, __LINE__, fmt, tisix::fmt_make_storage(__VA_ARGS__).args);
#define log_error(fmt, ...) log_impl(__FILENAME__, __LINE__, fmt, tisix::fmt_make_storage(__VA_ARGS__).args, tisix::LOG_ERROR);
} // namespace tisix