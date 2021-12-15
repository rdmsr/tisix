#include <tisix/mem.hpp>

bool tisix::memcmp(const void *a, const void *b, size_t s)
{

    int diff = *(char *)a - *(char *)b;

    for (size_t i = 0; i < s; i++)
    {
        if (diff != 0)
        {
            return false;
        }
    }

    return true;
}

void *tisix::memset(void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;

    size_t i;

    for (i = 0; i < n; i++)
    {
        p[i] = (uint8_t)c;
    }

    return s;
}

void tisix::memcpy(void *d, void *s, size_t n)
{
    char *csrc = (char *)s;
    char *cdest = (char *)d;

    for (size_t i = 0; i < n; i++)
        cdest[i] = csrc[i];
}

extern "C" void memcpy(void *d, void *s, size_t n)
{
    return tisix::memcpy(d, s, n);
}

extern "C" void atexit(void){};