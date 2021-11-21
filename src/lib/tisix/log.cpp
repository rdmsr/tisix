#include "loader.hpp"
#include "tisix/arch.hpp"
#include <tisix/alloc.hpp>
#include <tisix/host.hpp>
#include <tisix/log.hpp>

using namespace tisix;
static uint32_t lock = 0;

StringView get_name_from_logging_type(LogType t)
{
    switch (t)
    {
    case LOG_INFO:
        return "\033[1;32minfo\033[0m";
    case LOG_ERROR:
        return "\033[1;31merror\033[0m";
    case LOG_WARNING:
        return "\033[1;33mwarning\033[0m";
    }

    return "";
}

void tisix::log_impl(const char *file, int line, StringView fmt, FmtArgs args, LogType type)
{
    lock_acquire(&lock);

    fmt_stream(host_log_write, "{} \033[30m{}:{}\033[0m ", get_name_from_logging_type(type), file, line);

    fmt_stream_impl(host_log_write, fmt, args);

    host_log_write("\n");

    lock_release(&lock);
}