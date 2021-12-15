#pragma once
#include "tisix/arch.hpp"
#include "tisix/maybe.hpp"
#include "tisix/mem.hpp"
#include <tisix/host.hpp>
#include <tisix/std.hpp>

namespace tisix
{

struct AllocFreeNode
{
    AllocFreeNode *next;
};

class Alloc
{

    uint8_t *buf = nullptr;

    size_t chunk_size;
    size_t buf_size = 0;

public:
    Alloc(uint8_t *buf, size_t size, size_t chunk_size_ = 256) : buf(buf), chunk_size(chunk_size_), buf_size(size)
    {
        head = nullptr;

        this->free_all();
    }

    Alloc(){};

    void construct(uint8_t *buf_, size_t size, size_t chunk_size_ = 256)
    {
        buf = buf_;
        chunk_size = chunk_size_;
        buf_size = size;

        head = nullptr;
        this->free_all();
    }

    Maybe<void *> allocate(size_t _size)
    {
        if (_size > chunk_size)
        {
            panic("Size {} too big for chunk size {}", _size, chunk_size);
            return Nothing;
        }

        auto node = head;

        if (!node)
        {
            panic("No free memory");
            return Nothing;
        }

        head = head->next;

        memset(node, 0, chunk_size);

        return Just(node);
    }

    Maybe<bool> free(void *ptr)
    {
        auto start = buf;
        auto end = &buf[buf_size];

        if (!ptr)
            return Nothing;

        if (!(start <= ptr && ptr < end))
        {
            panic("memory out of bounds {#p} {#p} {#p}", ptr, (void *)start, (void *)end);
            return Nothing;
        }

        auto node = (AllocFreeNode *)ptr;

        node->next = head;

        head = node;

        return Just(true);
    }

    void free_all()
    {
        // Calculate number of chunks in alloc.
        size_t chunk_count = buf_size / chunk_size;

        for (size_t i = 0; i < chunk_count; i++)
        {
            void *ptr = &buf[i * chunk_size];

            auto node = (AllocFreeNode *)ptr;

            node->next = head;

            head = node;
        }
    }

    AllocFreeNode *head;
};

void *malloc(size_t size);
void free(void *ptr);

} // namespace tisix
