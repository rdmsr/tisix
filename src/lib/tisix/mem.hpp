#pragma once

#include <tisix/std.hpp>

namespace tisix
{
bool memcmp(const void *a, const void *b, size_t s);
void *memset(void *s, int c, size_t n);
void memcpy(void *d, void *s, size_t n);
} // namespace tisix