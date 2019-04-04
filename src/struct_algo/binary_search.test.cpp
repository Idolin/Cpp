#include "../debug/test_def.h"
#include "binary_search.hpp"

TEST_PACK(binary_search)
{
    TEST(int_type)
    {
        binary_search<int> b(0, 8, true);
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
        binary_search<int> b(0, 0, true);
        EXPECT_FALSE(b.done());
        EXPECT_EQ(b.current(), 0);
        b.right();
        EXPECT_TRUE(b.done());
    }

    TEST(bad_range)
    {
        binary_search<int> b(10, 9, true);
        EXPECT_TRUE(b.done());
        b.left();
        EXPECT_TRUE(b.done());
    }

    TEST(full_range)
    {
        binary_search<int> b(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), true);
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
        binary_search<int> b(std::numeric_limits<int>::max() - 72, std::numeric_limits<int>::max() - 46, true);
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
        binary_search<bool> b(false, true, true);
        EXPECT_FALSE(b.done());
        if(b.current())
            b.left();
        else
            b.right();
        EXPECT_FALSE(b.done());
        b.right();
        EXPECT_TRUE(b.done());

        binary_search<bool> b2(true, false, true);
        EXPECT_TRUE(b.done());
    }

    TEST(pointer_range)
    {
        int *a = new int[137];
        for(int i = 0;i < 137;i++)
            a[i] = i;
        binary_search<int*> b(a, a + 136, true);
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
        binary_search<float> b(-3, 3, true);
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
        binary_search<double> b(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), true);
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

    TEST(equals)
    {
        binary_search<int> a(1, 3, true);
        binary_search<int> b(1, 3, true);
        binary_search<int> c(1, 7, true);
        EXPECT_EQ(a, b);
        EXPECT_NE(b, c);
        a.left();
        EXPECT_NE(a, b);
        b.left();
        EXPECT_EQ(a, b);
        c.left();
        EXPECT_NE(b, c);
        c.right();
        c.right();
        EXPECT_TRUE(c.done());
        EXPECT_NE(a, c);
        a.left();
        EXPECT_TRUE(a.done());
        EXPECT_TRUE(b, "(b) == !b.done()");
        EXPECT_NE(a, c);
        EXPECT_NE(a, b);
        b.left();
        EXPECT_EQ(a, b);
        EXPECT_NE(b, c);
    }

    TEST(assign)
    {
        binary_search<int> a(1, 3);
        binary_search<int> b = a;
        binary_search<int> c(1, 5);
        EXPECT_EQ(a, b);
        EXPECT_NE(a, c);
        a.left();
        EXPECT_NE(a, b);
        c = b;
        EXPECT_NE(a, c);
        EXPECT_EQ(b, c);
        c.left();
        EXPECT_EQ(a, c);
        EXPECT_NE(a, b);
        EXPECT_NE(b, c);
    }

    TEST(lower_bound)
    {
        int a[] = {0, 0, 1, 2, 2, 4};
        binary_search<int*> b(&a[0], &a[sizeof(a) / sizeof(int)]);
        auto t = b;
        EXPECT_EQ(b.lower_bound(0) - (int*)&a, 0, "Expect 0, not %u", (b = t).lower_bound(0) - (int*)&a);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound(3) - (int*)&a, 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound(-9) - (int*)&a, 0);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound(4) - (int*)&a, 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound(5) - (int*)&a, 6);
        EXPECT_FALSE(b);
        b = t;

        EXPECT_EQ(b.lower_bound(5, true) - (int*)&a, 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound(0, true) - (int*)&a, -1);
        EXPECT_FALSE(b);
    }

    TEST(upper_bound)
    {
        int a[] = {0, 0, 1, 2, 2, 4};
        binary_search<int*> b(&a[0], &a[sizeof(a) / sizeof(int)]);
        auto t = b;
        EXPECT_EQ(b.upper_bound(0) - (int*)&a, 2, "Expect 2, not %u", (b = t).upper_bound(0) - (int*)&a);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound(3) - (int*)&a, 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound(-9) - (int*)&a, 0);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound(4) - (int*)&a, 6);
        EXPECT_FALSE(b);
        b = t;

        EXPECT_EQ(b.upper_bound(5) - (int*)&a, 6);
        EXPECT_FALSE(b);
        b = t;

        EXPECT_EQ(b.upper_bound(5, true) - (int*)&a, 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound(0, true) - (int*)&a, 1);
        EXPECT_TRUE(b);
    }

    TEST(bound_pos)
    {
        int a[] = {0, 0, 1, 2, 2, 4};
        binary_search<int*> b(&a[0], &a[sizeof(a) / sizeof(int)]);
        auto t = b;

        EXPECT_EQ(b.upper_bound_pos(0), 2, "Expect 2, not %u", (b = t).upper_bound_pos(0));
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound_pos(3), 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound_pos(-9), 0);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound_pos(4), 6);
        EXPECT_FALSE(b);
        b = t;

        EXPECT_EQ(b.lower_bound_pos(0), 0, "Expect 0, not %u", (b = t).lower_bound_pos(0));
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound_pos(3), 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound_pos(-9), 0);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound_pos(4), 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound_pos(5, true), 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.lower_bound_pos(0, true), -1);
        EXPECT_FALSE(b);
        b = t;

        EXPECT_EQ(b.upper_bound_pos(5, true), 5);
        EXPECT_TRUE(b);
        b = t;

        EXPECT_EQ(b.upper_bound_pos(0, true), 1);
        EXPECT_TRUE(b);
    }

    TEST(bounds_check)
    {
        int a[] = {0, 0, 1, 2, 2, 4};
        binary_search<int*> b(&a[0], &a[sizeof(a) / sizeof(int)]);
        auto t = b;

        auto r = b.upper_bound_pos_check(0);
        EXPECT_EQ(r.first, 2, "Expect 2, not %u", r.first);
        EXPECT_TRUE(r.second, "Expect true, not false");
        b = t;

        r = b.lower_bound_pos_check(0);
        EXPECT_EQ(r.first, 0, "Expect 0, not %u", r.first);
        EXPECT_TRUE(r.second, "Expect true, not false");
        b = t;

        r = b.upper_bound_pos_check(4);
        EXPECT_EQ(r.first, 6, "Expect 6, not %u", r.first);
        EXPECT_FALSE(r.second, "Expect false, not true");
        b = t;

        r = b.lower_bound_pos_check(5);
        EXPECT_EQ(r.first, 6, "Expect 6, not %u", r.first);
        EXPECT_FALSE(r.second, "Expect false, not true");
        b = t;

        auto r2 = b.upper_bound_check(0);
        EXPECT_EQ(r2.first - (int*)&a, 2, "Expect &a[2], not &a[%u]", r2.first - (int*)&a);
        EXPECT_TRUE(r2.second, "Expect true, not false");
        b = t;

        r2 = b.lower_bound_check(0);
        EXPECT_EQ(r2.first - (int*)&a, 0, "Expect &a[0], not &a[%u]", r2.first);
        EXPECT_TRUE(r2.second, "Expect true, not false");
        b = t;

        r2 = b.upper_bound_check(4);
        EXPECT_EQ(r2.first - (int*)&a, 6, "Expect &a[6], not &a[%u]", r2.first);
        EXPECT_FALSE(r2.second, "Expect false, not true");
        b = t;

        r2 = b.lower_bound_check(5);
        EXPECT_EQ(r2.first - (int*)&a, 6, "Expect &a[6], not &a[%u]", r2.first);
        EXPECT_FALSE(r2.second, "Expect false, not true");
        b = t;
    }

    TEST(bounds)
    {
        int a[] = {0, 0, 1, 2, 2, 4};
        binary_search<int*> b(&a[0], &a[sizeof(a) / sizeof(int)]);
        auto t = b;

        auto r = b.bounds(-1);
        EXPECT_GT(r.first, r.second);
        EXPECT_FALSE(b);
        b = t;

        r = b.bounds(0);
        EXPECT_EQ(r.first - (int*)&a, 0);
        EXPECT_EQ(r.second - (int*)&a, 1);
        EXPECT_TRUE(b);
        b = t;

        r = b.bounds(3);
        EXPECT_GT(r.first, r.second);
        EXPECT_FALSE(b);
        b = t;

        r = b.bounds(4);
        EXPECT_EQ(r.first - (int*)&a, 5);
        EXPECT_EQ(r.second - (int*)&a, 5);
        EXPECT_TRUE(b);
        b = t;

        r = b.bounds(5);
        EXPECT_GT(r.first, r.second);
        EXPECT_FALSE(b);
        b = t;

        auto r2 = b.bounds_pos(-1);
        EXPECT_GT(r2.first, r2.second);
        EXPECT_FALSE(b);
        b = t;

        r2 = b.bounds_pos(0);
        EXPECT_EQ(r2.first, 0);
        EXPECT_EQ(r2.second, 1);
        EXPECT_TRUE(b);
        b = t;

        r2 = b.bounds_pos(3);
        EXPECT_GT(r2.first, r2.second);
        EXPECT_FALSE(b);
        b = t;

        r2 = b.bounds_pos(4);
        EXPECT_EQ(r2.first, 5);
        EXPECT_EQ(r2.second, 5);
        EXPECT_TRUE(b);
        b = t;

        r2 = b.bounds_pos(5);
        EXPECT_GT(r2.first, r2.second);
        EXPECT_FALSE(b);
    }
}
