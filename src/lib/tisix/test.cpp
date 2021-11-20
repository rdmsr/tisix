#include "tisix/alloc.hpp"
#include <tisix/log.hpp>
#include <tisix/test.hpp>

namespace tisix
{
static size_t test_count = 0;
static size_t fail_count = 0;
static size_t success_count = 0;
static Test tests[1024] = {};
static int last_fail_line = 0;
static const char *last_fail_file, *last_fail_test;

void test_add(Test test)
{
    tests[test_count++] = test;
}

void unit_assert(bool val, int line, const char *file, const char *equality)
{
    if (!val)
    {
        last_fail_line = line;
        last_fail_file = file;
        last_fail_test = equality;
        fail_count++;
    }
    else
    {
        success_count++;
    }

    return;
}

int test_run(Test test)
{
    size_t previous_fail_count = fail_count;

    test.func();

    if (fail_count > previous_fail_count)
    {
        return -1;
    }

    return 0;
}

int test_run_all(void)
{
    log("====== Running {} tests ======", test_count);

    for (size_t i = 0; i < test_count; i++)
    {
        auto test = tests[i];
        auto result = test_run(test);

        if (result == 0)
        {
            log("[ ✓ ] {}", test.name);
        }

        if (result == -1)
        {
            log_error("[ ✗ ] test \"{}\" failed at {}:{}, condition was \"{}\"", test.name, (const char *)last_fail_file, last_fail_line, last_fail_test);
        }
    }

    log("====== Statistics: {} successes | {} fails ======", success_count, fail_count);

    return fail_count > 0 ? -1 : 0;
}
}; // namespace tisix