#include <tisix/mem.hpp>
#include <tisix/string_view.hpp>

using namespace tisix;

static size_t strlen(const char *s)
{
    size_t i;
    for (i = 0; s[i] != '\0'; i++)
        ;
    return i;
}

StringView::StringView(const char *s)
{
    this->buffer = s;

    this->size = strlen(s);
}

StringView::StringView()
{
    this->buffer = 0;
    this->size = 0;
}

bool StringView::operator==(const char *s)
{
    return memcmp(this->buffer, s, this->size);
}

bool StringView::operator==(StringView v)
{
    return memcmp(this->buffer, v.buffer, this->size);
}

char StringView::operator[](size_t index)
{
    return buffer[index];
}

const char *StringView::begin()
{
    return this->buffer;
}

const char *StringView::end()
{
    return this->buffer + this->size;
}

template <typename T>
StringView to_str(T arg, int base)
{
    static char digits[] = "0123456789ABCDEF";
    static char buffer[50];
    char *str;

    str = &buffer[49];
    *str = '\0';

    do
    {
        *--str = digits[arg % base];
        arg /= base;
    } while (arg);

    return str;
}

// https://github.com/Keyboard-Slayer/experimental/blob/main/src/lib/experimental/itoa.c
static void strrev(char *s)
{
    char tmp;
    size_t end = strlen(s) - 1;
    size_t start = 0;

    while (start < end)
    {
        tmp = s[start];
        s[start++] = s[end];
        s[end--] = tmp;
    }
}

char *tisix::itoa(int64_t value, char *str, uint16_t base)
{
    size_t index = 0;
    int8_t digit;

    if (value < 0 && base != 10)
    {
        int64_t left;
        int64_t right;
        size_t i;

        char left_str[32];
        char right_str[32];

        right = value & 0xffffffff;
        left = (value & 0xffffffff00000000) >> 32;

        tisix::itoa(left, left_str, base);
        tisix::itoa(right, right_str, base);

        for (i = 0; i < strlen(left_str); i++)
        {
            str[i] = left_str[i];
        }

        for (i = 0; i < strlen(right_str); i++)
        {
            str[i + strlen(left_str)] = right_str[i];
        }

        return str;
    }

    else if (value < 0)
    {
        str[index++] = '-';
        value *= 1;
    }

    do
    {
        digit = value % base;

        if (digit < 0xa)
        {
            str[index++] = digit + '0';
        }
        else
        {
            str[index++] = (digit - 0xa) + 'a';
        }

        value /= base;
    } while (value);

    str[index] = 0;

    strrev(str);

    return str;
}

StringView to_str(const char *str)
{
    return str;
}

extern "C" char *strrchr(char const *s, int c)
{
    size_t length = strlen(s);
    for (size_t i = 0; i < length; i++)
    {
        if (s[i] == c)
        {
            return (char *)s + i;
        }
    }

    return nullptr;
}
