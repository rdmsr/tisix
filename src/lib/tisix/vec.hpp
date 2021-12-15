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
    Vec(size_t cap = 3)
    {
        this->capacity = cap;
        this->size = 0;
        this->data = (T *)tisix::malloc(this->capacity * sizeof(*this->data));

        if (!this->data)
            panic("Couldn't allocate memory for dynamic array");
    }

    void construct(size_t cap = 3)
    {
        this->capacity = cap;
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

    void remove(size_t index)
    {
        for (size_t i = index - 1; i < this->size; ++i)
            data[i] = data[i + 1];

        this->size--;
    }

public:
    size_t size;

    T *data;

private:
    size_t capacity;

private:
    void resize()
    {
        T *tmp = (T *)tisix::malloc(this->capacity * sizeof(*this->data));

        this->capacity *= sizeof(*this->data);

        memcpy(tmp, data, this->size);

        if (!tmp)
            panic("Couldn't realloc() dynamic array");

        this->data = tmp;
        this->capacity = capacity;
    }
};

} // namespace tisix