#include "fmt.hpp"
#include "tisix/string_view.hpp"
#include <tisix/scan.hpp>

using namespace tisix;

using os = void (*)(const char *s);

void fmt_print_value(os output, FmtValue v, int base = 10, size_t pad = 0)
{
    switch (v.type)
    {

    case FMT_STRING:
    {
        output(v.string.buffer);
        break;
    }
    case FMT_BOOL:
    {
        output(v._bool ? "#t" : "#f");
        break;
    }

    case FMT_SIGNED:
    {
        char buf[50];

        if (base == 16)
            output("0x");

        itoa(v._signed, buf, base);

        if (StringView(buf).size < pad)
        {
            for (size_t i = 0; i < pad - StringView(buf).size; i++)
            {
                char s[] = {'0', 0};
                output(s);
            }
        }

        output(buf);
        break;
    }

    case FMT_UNSIGNED:
    {

        char buf[50];

        if (base == 16)
            output("0x");

        itoa(v._unsigned, buf, base);

        if (StringView(buf).size < pad)
        {
            for (size_t i = 0; i < pad - StringView(buf).size; i++)
            {
                char s[] = {'0', 0};
                output(s);
            }
        }

        output(buf);
        break;
    }

    case FMT_POINTER:
    {
        char buf[50];

        output("0x");

        output(itoa((uintptr_t)v.ptr, buf, 16));
        break;
    }

    case FMT_CHAR:
    {

        char s[] = {v._char, 0};
        output(s);
        break;
    }

    default:
        break;
    }
}
void tisix::fmt_stream_impl(os stream, StringView fmt, FmtArgs args)
{
    Scanner scan(fmt);

    size_t current = 0;

    while (!scan.ended())
    {
        if (scan.current() == '{' && scan.peek(1) == '}')
        {
            fmt_print_value(stream, args.values[current]);

            current++;

            scan.forward();
        }

        else if (scan.current() == '{' && scan.peek(1) == '#')
        {
            scan.forward(2);

            switch (scan.current())
            {
            case 'x':
            {
                fmt_print_value(stream, args.values[current], 16);
                current++;
                break;
            }

            case 'p':
            {
                fmt_print_value(stream, args.values[current], 16, 16);
                current++;
                break;
            }
            }

            scan.forward();
        }

        else if (scan.peek(1) == '@')
        {
            scan.forward(2);

            if (scan.current() <= '9' && scan.current() >= '0')
            {
                fmt_print_value(stream, args.values[scan.current() - '0']);
            }

            scan.forward();
        }

        else
        {
            char s[] = {scan.current(), 0};
            stream(s);
        }

        scan.forward();
    }
}