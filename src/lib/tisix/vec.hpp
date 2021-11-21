#pragma once

// Dynamic Array

#include <tisix/alloc.hpp>
#include <tisix/arch.hpp>

namespace tisix
{

template <typename T>
class Vec
{
public:
    Vec()
    {
        this->capacity = 3;
        this->size = 0;
        this->data = (T *)tisix::malloc(this->capacity * sizeof(*this->data));

        if (!this->data)
            panic("Couldn't allocate memory for dynamic array");
    }

    void construct()
    {
        this->capacity = 3;
        this->size = 0;
        this->data = (T *)tisix::malloc(this->capacity * sizeof(*this->data));

        if (!this->data)
            panic("Couldn't allocate memory for dynamic array");
    }

    ~Vec()
    {
        free(this->data);
    }

    void push(T value)
    {
        if (this->size > this->capacity)
            this->resize();

        this->data[this->size++] = value;
    }

    T pop()
    {
        return this->data[--this->size];
    }

    void set(size_t index, T value)
    {
        while (index > this->size)
            this->push(0);

        this->data[index] = value;
    }

    T &operator[](int index) { return data[index]; };

    T *end() { return data + size; };
    T *begin() { return data; };

public:
    size_t size;

    T *data;

private:
    size_t capacity;

private:
    void resize()
    {
        size_t capacity = this->capacity * 2;

        T *tmp = (T *)tisix::realloc(this->data, capacity * sizeof(*this->data));

        if (!tmp)
            panic("Couldn't realloc() dynamic array");

        this->data = tmp;
        this->capacity = capacity;
    }
};

} // namespace tisix