#include "tisix/alloc.hpp"
#include "tisix/log.hpp"
#include "tisix/std.hpp"
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <tisix/arch.hpp>
#include <tisix/test.hpp>

/* Arch stuff ----------- */
using tisix::Maybe;

static Arch arch = (Arch){.debug_stream = nullptr, .name = "host", .allocator = nullptr};

void lock_acquire(uint32_t *lock) { (void)lock; };
void lock_release(uint32_t *lock) { (void)lock; };

Arch *get_arch()
{
    return &arch;
}

class HostAlloc : public MemoryAllocator
{
public:
    void free(void *addr, size_t pages) override
    {
        std::free(addr);
        (void)pages;
    }

    Maybe<void *> allocate(size_t n) override
    {
        return Just(std::malloc(n));
    }
};

void arch_panic_impl(const char *file, int line, tisix::StringView fmt, tisix::FmtArgs args)
{
    log_error("Panic at {}:{}", file, line);

    tisix::log_impl(file, line, fmt, args);
}

class ArchWriter : public tisix::Stream<char const *>
{
public:
    void write(char const *text) override
    {
        printf("%s", text);
    }
    char const *read() override
    {
        return 0;
    }

    void putc(char c) override
    {
        std::putc(c, stdout);
    }
};

int main()
{
    ArchWriter writer;

    arch.debug_stream = &writer;

    return tisix::test_run_all();
}