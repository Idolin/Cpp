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
        unsigned *t = new unsigned[10000];
        FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, sort_algo<unsigned> sort,
            bubblesort, shakersort, minmaxsort, insertionsort, {
            SUB_TEST(reverse_order)
            {
                FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, {
                    for(unsigned i = 0; i < length; i++)
                        t[i] = length - i;
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(sorted_order)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, {
                     for(unsigned i = 0; i < length; i++)
                        t[i] = i;
                     sort(t, t + length);
                     EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(few_numbers)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, {
                    for(unsigned i = 0; i < length; i++)
                        t[i] = randomU() & 0x0f;
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(partially_sorted)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, {
                    for(unsigned i = 0, k = 0; i < length; i++, k += (i & 0x40 == 0x40) * 30)
                        t[i] = i + 20 + randomU() & 0xff - randomU() & 0xf - k;
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(fully_random)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, {
                    for(unsigned i = 0; i < length; i++)
                        t[i] = randomU();
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
        });
        delete [] t;
    }

    bool compare_short_reversed(short a, short b)
    {
        return (a > b);
    }

    bool compare_unsigned_long_long_odd_first(unsigned long long a, unsigned long long b)
    {
        return ((a ^ b) & 0x1) ? (a & 0x1) : (a < b);
    }

    TEST(square_sort_custom_comparator)
    {
        short *t = new short[10000];
        unsigned long long *l = new unsigned long long[10000];
        FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, sort_algo<short> sort,
                (bubblesort<short, compare_short_reversed>),
                (shakersort<short, compare_short_reversed>),
                (minmaxsort<short, compare_short_reversed>),
                (insertionsort<short, compare_short_reversed>), {
	        SUB_TEST(short_reversed)
	        {
	            FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, {
	                for(unsigned i = 0; i < length; i++)
	                    t[i] = randomS();
	                sort(t, t + length);
	                EXPECT_TRUE((_checksorted<short, compare_short_reversed>(t, t + length)));
	            });
	        }
        });
        FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, sort_algo<unsigned long long> sort,
                (bubblesort<unsigned long long, compare_unsigned_long_long_odd_first>),
                (shakersort<unsigned long long, compare_unsigned_long_long_odd_first>),
                (minmaxsort<unsigned long long, compare_unsigned_long_long_odd_first>),
                (insertionsort<unsigned long long, compare_unsigned_long_long_odd_first>), {
	        SUB_TEST(unsigned_long_long_odd_first)
	        {
	            FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, {
	                for(unsigned i = 0; i < length; i++)
	                    l[i] = randomULL();
	                sort(l, l + length);
	                EXPECT_TRUE((_checksorted<unsigned long long, compare_unsigned_long_long_odd_first>(l, l + length)));
	            });
	        }
        });
        delete [] t;
        delete [] l;
	}

	TEST(sort_n_logn)
    {
    	unsigned *t = new unsigned[10000000];
        FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, sort_algo<unsigned> sort,
            mergesort, quicksort, {
            SUB_TEST(reverse_order)
            {
                FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, {
                    for(unsigned i = 0; i < length; i++)
                        t[i] = length - i;
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(sorted_order)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, {
                     for(unsigned i = 0; i < length; i++)
                        t[i] = i;
                     sort(t, t + length);
                     EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(few_numbers)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, {
                    for(unsigned i = 0; i < length; i++)
                        t[i] = randomU() & 0x0f;
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(partially_sorted)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, {
                    for(unsigned i = 0, k = 0; i < length; i++, k += (i & 0x40 == 0x40) * 30)
                        t[i] = i + 20 + randomU() & 0xff - randomU() & 0xf - k;
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
            SUB_TEST(fully_random)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, {
                    for(unsigned i = 0; i < length; i++)
                        t[i] = randomU();
                    sort(t, t + length);
                    EXPECT_TRUE(_checksorted(t, t + length));
                });
            }
        });
    }

    TEST(n_logn_sort_custom_comparator)
	{
	    short *t = new short[10000000];
	    unsigned long long *l = new unsigned long long[10000000];
	    FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, sort_algo<short> sort,
	            (mergesort<short, compare_short_reversed>),
	            (quicksort<short, compare_short_reversed>), {
            SUB_TEST(short_reversed)
            {
                FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned
                length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, {
                        for(unsigned i = 0; i < length; i++)
                            t[i] = randomS();
                        sort(t, t + length);
                        EXPECT_TRUE((_checksorted<short, compare_short_reversed>(t, t + length)));
                });
            }
        });
	    FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, sort_algo<unsigned long long> sort,
	            (mergesort<unsigned long long, compare_unsigned_long_long_odd_first>),
	            (quicksort<unsigned long long, compare_unsigned_long_long_odd_first>), {
	        SUB_TEST(unsigned_long_long_odd_first)
	        {
	            FOR_EACH_ARG_COMPOSE_I(COMPOSE_TEST, unsigned
	            length, 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, {
	                    for(unsigned i = 0; i < length; i++)
	                        l[i] = randomULL();
	                    sort(l, l + length);
	                    EXPECT_TRUE((_checksorted<unsigned long long, compare_unsigned_long_long_odd_first>(l, l + length)));
	            });
	        }
	    });
	    delete [] t;
	    delete [] l;
	}
}