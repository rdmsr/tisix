#include <tisix/scan.hpp>

using namespace tisix;

char Scanner::peek(size_t offset)
{
    if (this->head + offset >= this->size)
    {
        return '\0';
    }

    return this->buffer[this->head + offset];
}

char Scanner::forward()
{
    if (this->ended())
        return 0;

    this->head++;

    return this->current();
}

char Scanner::forward(size_t n)
{
    if (this->ended())
        return 0;

    this->head += n;

    return this->current();
}

char Scanner::current()
{
    return this->peek(0);
}

bool Scanner::ended()
{
    return this->head + 1 > this->size;
}

Scanner::Scanner(StringView s)
{
    this->size = s.size;
    this->buffer = s.buffer;
    this->head = 0;
}