#include "tisix/scan.hpp"
#include <tisix/alloc.hpp>
#include <tisix/list.hpp>
#include <tisix/test.hpp>
#include <tisix/vec.hpp>

TEST(Success)
{
    UNIT_ASSERT(1 == 1);
}

TEST(Malloc)
{
    void *test = tisix::malloc(100);

    UNIT_ASSERT(test != NULL);
}

TEST(StringView)
{
    tisix::StringView str = "IAmAStringThatIs32CharactersLong";
    tisix::StringView str2 = "IAmAStringThatIs32CharactersLong";

    UNIT_ASSERT(str.size == 32);

    UNIT_ASSERT(str == str2);

    UNIT_ASSERT(str[0] == 'I');
}

TEST(Scanner)
{
    tisix::StringView str = "hello";

    tisix::Scanner scan(str);

    UNIT_ASSERT(scan.current() == 'h');

    scan.forward();

    UNIT_ASSERT(scan.current() == 'e');

    scan.forward();

    UNIT_ASSERT(scan.current() == 'l');

    scan.forward();

    UNIT_ASSERT(scan.current() == 'l');

    scan.forward();

    UNIT_ASSERT(scan.current() == 'o');

    scan.forward();

    UNIT_ASSERT(scan.ended() == true);
}

TEST(Vec)
{
    tisix::Vec<int> some_vec;

    UNIT_ASSERT(some_vec.size == 0);

    some_vec.push(10);

    UNIT_ASSERT(some_vec.size == 1);

    UNIT_ASSERT(some_vec[0] == 10);

    some_vec.pop();

    UNIT_ASSERT(some_vec.size == 0);
}

TEST(List)
{
    tisix::List<int> some_list;

    some_list.push(10);

    UNIT_ASSERT(some_list[0] == 10);
    UNIT_ASSERT(some_list.size == 1);
    UNIT_ASSERT(some_list[0] == some_list.head->data);
}