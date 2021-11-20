#pragma once
#include <tisix/string_view.hpp>

namespace tisix
{

#define TEST_FUNCTION_NAME(name) test_##name

struct Test
{
    const char *name;
    void (*func)(void);
};

void test_add(Test test);

#define TEST(fname)                                                              \
    void TEST_FUNCTION_NAME(fname)(void);                                        \
    [[gnu::constructor]] static inline void test_register_##fname##_detail(void) \
    {                                                                            \
        tisix::test_add((tisix::Test){#fname, TEST_FUNCTION_NAME(fname)});       \
    }                                                                            \
    void TEST_FUNCTION_NAME(fname)(void)

int test_run_all(void);
#define GET_PARAMETER_STRING(val) #val
#define UNIT_ASSERT(val) tisix::unit_assert(val, __LINE__, __FILENAME__, #val)
#define UNIT_ASSERT_EQUAL(a, b) tisix::unit_assert(a == b, __LINE__, __FILENAME__, GET_PARAMETER_STRING(a == b))

void unit_assert(bool val, int line, const char *file, const char *equality);

} // namespace tisix