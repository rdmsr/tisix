#pragma once
#include <tisix/std.hpp>

namespace tisix
{
class StringBuilder 
{
    public:
        StringBuilder(const char* s);
        ~StringBuilder();
    public:
        char* operator +(const char* s);

    public:
        char* buffer;

};
}