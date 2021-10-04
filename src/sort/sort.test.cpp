#include "../debug/test_def.h"
#include "sortsquare.hpp"
#include "sortlog.hpp"
#include "sortline.hpp"
#include "../template/arraymethods.hpp"
#include "../other/defdef.h"
#include "../other/rand.h"
#include "../template/typemethods.hpp"

TEST_PACK(sort)
{
    template<typename T>
	using sort_algo = void(*)(T*, T*);

    TEST(square_sort_unsigned)
    {
        auto u = new unsigned[10000];
        WITH_VALUES(sort_algo<unsigned>, sort,
            bubblesort, shakersort, minmaxsort, insertionsort)
        {
            SUBTEST(reverse_order)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000)
                {
                    for(unsigned i = 0; i < length; i++)
                        u[i] = length - i;
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(sorted_order)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000)
                {
                     for(unsigned i = 0; i < length; i++)
                        u[i] = i;
                     sort(u, u + length);
                     EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(few_numbers)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000)
                {
                    for(unsigned i = 0; i < length; i++)
                        u[i] = randomU() & 0x0fu;
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(partially_sorted)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000)
                {
                    for(unsigned i = 0, k = 0; i < length; i++, k += ((i & 0x40) == 0x40) * 30)
                        u[i] = i + 20 + (randomU() & 0xff) - (randomU() & 0xf) - k;
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(fully_random)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000)
                {
                    for(unsigned i = 0; i < length; i++)
                        u[i] = randomU();
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
        };
        delete [] u;
    }

    bool compare_short_reversed(short a, short b)
    {
        return (a > b);
    }

    bool compare_unsigned_long_long_odd_first(unsigned long long a, unsigned long long b)
    {
        return (((a ^ b) & 0x1u)) ? ((a & 0x1u)) : (a < b);
    }

    TEST(square_sort_custom_comparator)
    {
        auto s = new short[10000];
        auto ull = new unsigned long long[10000];
        WITH_VALUES(sort_algo<short>, sort,
                bubblesort<short, compare_short_reversed>,
                shakersort<short, compare_short_reversed>,
                minmaxsort<short, compare_short_reversed>,
                insertionsort<short, compare_short_reversed>)
        {
	        SUBTEST(short_reversed)
	        {
	            WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000)
                {
	                for(unsigned i = 0; i < length; i++)
	                    s[i] = randomS();
	                sort(s, s + length);
	                EXPECT_TRUE((_checksorted<short, compare_short_reversed>(s, s + length)));
	            };
	        };
        };
        WITH_VALUES(sort_algo<unsigned long long>, sort,
                bubblesort<unsigned long long, compare_unsigned_long_long_odd_first>,
                shakersort<unsigned long long, compare_unsigned_long_long_odd_first>,
                minmaxsort<unsigned long long, compare_unsigned_long_long_odd_first>,
                insertionsort<unsigned long long, compare_unsigned_long_long_odd_first>)
        {
	        SUBTEST(unsigned_long_long_odd_first)
	        {
	            WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000)
                {
	                for(unsigned i = 0; i < length; i++)
	                    ull[i] = randomULL();
	                sort(ull, ull + length);
	                EXPECT_TRUE((_checksorted<unsigned long long, compare_unsigned_long_long_odd_first>(ull, ull + length)));
	            };
	        };
        };
        delete [] s;
        delete [] ull;
	}

	TEST(sort_n_logn_unsigned)
    {
    	auto u = new unsigned[10000000];
        WITH_VALUES(sort_algo<unsigned>, sort, mergesort, quicksort)
        {
            SUBTEST(reverse_order)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000) 
                {
                    for(unsigned i = 0; i < length; i++)
                        u[i] = length - i;
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(sorted_order)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
                {
                     for(unsigned i = 0; i < length; i++)
                        u[i] = i;
                     sort(u, u + length);
                     EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(few_numbers)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
                {
                    for(unsigned i = 0; i < length; i++)
                        u[i] = randomU() & 0x0fu;
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(partially_sorted)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
                {
                    for(unsigned i = 0, k = 0; i < length; i++, k += ((i & 0x40) == 0x40) * 30)
                        u[i] = i + 20 + (randomU() & 0xff) - (randomU() & 0xf) - k;
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
            SUBTEST(fully_random)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
                {
                    for(unsigned i = 0; i < length; i++)
                        u[i] = randomU();
                    sort(u, u + length);
                    EXPECT_TRUE(_checksorted(u, u + length));
                };
            };
        };
        delete[] u;
    }

    TEST(n_logn_sort_custom_comparator)
	{
	    auto s = new short[10000000];
	    auto ull = new unsigned long long[10000000];
	    WITH_VALUES(sort_algo<short>, sort,
	            mergesort<short, compare_short_reversed>,
	            quicksort<short, compare_short_reversed>)
        {
            SUBTEST(short_reversed)
            {
                WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
                {
                        for(unsigned i = 0; i < length; i++)
                            s[i] = randomS();
                        sort(s, s + length);
                        EXPECT_TRUE((_checksorted<short, compare_short_reversed>(s, s + length)));
                };
            };
        };
	    WITH_VALUES(sort_algo<unsigned long long>, sort,
	            mergesort<unsigned long long, compare_unsigned_long_long_odd_first>,
	            quicksort<unsigned long long, compare_unsigned_long_long_odd_first>)
        {
	        SUBTEST(unsigned_long_long_odd_first)
	        {
	            WITH_VALUES(unsigned, length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
                {
	                    for(unsigned i = 0; i < length; i++)
	                        ull[i] = randomULL();
	                    sort(ull, ull + length);
	                    EXPECT_TRUE((_checksorted<unsigned long long, compare_unsigned_long_long_odd_first>(ull, ull + length)));
	            };
	        };
	    };
	    delete [] s;
	    delete [] ull;
	}

	TEST(line_sort_unsigned, TEST_INFO_STR("with sort_algo<unsigned> sort = bucketsort<unsigned>"))
	{
		auto u = new unsigned[1000000000];
	    SUBTEST(equal_numbers)
        {
            WITH_VALUES(unsigned, length, 0, SEQ_10_POW(9))
            {
                for(unsigned i = 0; i < length; i++)
                    u[i] = length;
                bucketsort(u, u + length);
                EXPECT_TRUE(_checksorted(u, u + length));
            };
        };
        SUBTEST(few_numbers)
        {
            WITH_VALUES(unsigned, length, 0, SEQ_10_POW(9))
            {
                for(unsigned i = 0; i < length; i++)
                    u[i] = randomU() & 0x0fu;
                bucketsort(u, u + length);
                EXPECT_TRUE(_checksorted(u, u + length));
            };
        };
        SUBTEST(many_numbers)
        {
            WITH_VALUES(unsigned, length, 0, SEQ_10_POW(9))
            {
                for(unsigned i = 0; i < length; i++)
                    u[i] = randomU() & 0xffff;
                bucketsort(u, u + length);
                EXPECT_TRUE(_checksorted(u, u + length));
            };
        };
        SUBTEST(all_numbers_different)
        {
            WITH_VALUES(unsigned, length, 0, SEQ_10_POW(9))
            {
                for(unsigned i = 0; i < length; i++)
                    u[i] = i;
                bucketsort(u, u + length);
                EXPECT_TRUE(_checksorted(u, u + length));
            };
        };
        delete[] u;
	}
}
