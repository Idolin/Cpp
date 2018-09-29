#include "../debug/test_def.h"
#include "binary_search.hpp"

TEST_PACK(binary_search)
{
    TEST(int_type)
    {
        binary_search<int> b(0, 8);
        EXPECT_FALSE(b.done());
        EXPECT_EQ(b.current(), 4);
        b.left();
        EXPECT_FALSE(b.done());
        EXPECT_IN_RANGE(b.current(), 1, 2);
        b.left();
        EXPECT_FALSE(b.done());
        EXPECT_IN_RANGE(b.current(), 0, 1);
        if(b.current())
            b.right();
        else
            b.left();
        EXPECT_TRUE(b.done());
        b.left();
        EXPECT_TRUE(b.done());
        b.right();
        EXPECT_TRUE(b.done());
    }

    TEST(empty_range)
    {
        binary_search<int> b(0, 0);
        EXPECT_FALSE(b.done());
        EXPECT_EQ(b.current(), 0);
        b.right();
        EXPECT_TRUE(b.done());
    }

    TEST(bad_range)
    {
        binary_search<int> b(10, 9);
        EXPECT_TRUE(b.done());
        b.left();
        EXPECT_TRUE(b.done());
    }

    TEST(full_range)
    {
        binary_search<int> b(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());
        EXPECT_FALSE(b.done());
        unsigned n = 0;
        unsigned m = sizeof(int) * 8;
        while(!b.done() && n < m + 1)
        {
            b.left();
            n++;
        }
        EXPECT_IN_RANGE(n, m - 1, m);
    }

    TEST(range_near_right_border)
    {
        binary_search<int> b(std::numeric_limits<int>::max() - 72, std::numeric_limits<int>::max() - 46);
        EXPECT_FALSE(b.done());
        unsigned n = 0;
        unsigned m = 5;
        while(!b.done() && n < m + 1)
        {
            b.right();
            n++;
        }
        EXPECT_IN_RANGE(n, m - 2, m);
    }

    TEST(bool_range)
    {
        binary_search<bool> b(false, true);
        EXPECT_FALSE(b.done());
        if(b.current())
            b.left();
        else
            b.right();
        EXPECT_FALSE(b.done());
        b.right();
        EXPECT_TRUE(b.done());

        binary_search<bool> b2(true, false);
        EXPECT_TRUE(b.done());
    }

    TEST(pointer_range)
    {
        int *a = new int[137];
        for(int i = 0;i < 137;i++)
            a[i] = i;
        binary_search<int*> b(a, a + 136);
        EXPECT_FALSE(b.done());
        EXPECT_EQ(*b.current(), 68);
        unsigned n = 0;
        unsigned m = 8;
        while(!b.done() && n < m + 1)
        {
            b.right();
            n++;
        }
        EXPECT_IN_RANGE(n, m - 1, m);
    }

    TEST(float_range)
    {
        binary_search<float> b(-3, 3);
        EXPECT_FALSE(b.done());
        EXPECT_FLOAT_EQ(b.current(), 0);
        unsigned n = 0;
        unsigned m = sizeof(float) * 16;
        while(!b.done() && n < m)
        {
            b.right();
            n++;
        }
        EXPECT_IN_RANGE(n, m / 4 - 1, m);
    }

    TEST(full_double_range)
    {
        binary_search<double> b(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
        EXPECT_FALSE(b.done());
        EXPECT_DOUBLE_EQ(b.current(), 0, "Expected ~0 but got %.5lf\n", b.current());
        unsigned n = 0;
        unsigned m = sizeof(double) * 16;
        while(!b.done() && n < m)
        {
            b.right();
            n++;
        }
        EXPECT_IN_RANGE(n, m / 4 - 1, m, "Expected amount of steps(%u) to be in range: %u - %u", n, m / 2 - 1, m);
    }
}
