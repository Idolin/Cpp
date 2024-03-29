#include "../debug/test_def.h"
#include "commonmethods.hpp"

TEST_PACK(commonmethods)
{
    TEST(def_less)
    {
        EXPECT_TRUE(def_less(3, 5));
        EXPECT_TRUE(def_less(-5, -3));
        EXPECT_FALSE(def_less(0, -3));
        EXPECT_FALSE(def_less(4, 4));
    }

    TEST(def_more)
    {
        EXPECT_FALSE(def_more(3, 5));
        EXPECT_FALSE(def_more(-5, -3));
        EXPECT_TRUE(def_more(0, -3));
        EXPECT_FALSE(def_more(4, 4));
    }

    TEST(min)
    {
        int a = -90;
        unsigned char b = 0;
        signed char c = -100;
        long x = 93;
        EXPECT_EQ(min(a, b), -90);
        EXPECT_EQ(min(b, a), -90);
        EXPECT_EQ(min(a, c), -100);
        EXPECT_EQ(min(b, c), -100);
        EXPECT_EQ(min(c, a), -100);
        EXPECT_EQ(min(c, b), -100);
        EXPECT_EQ(min(b, x), 0);
        EXPECT_EQ(min(x, a), -90);
        EXPECT_EQ(min(x, x), 93);
        EXPECT_EQ(min(x, b), 0);
        EXPECT_EQ(min(c, x), -100);
    }

    TEST(max)
    {
        int a = -90;
        unsigned char b = 0;
        signed char c = -100;
        long x = 93;
        EXPECT_EQ(max(a, b), 0);
        EXPECT_EQ(max(b, a), 0);
        EXPECT_EQ(max(a, c), -90);
        EXPECT_EQ(max(b, c), 0);
        EXPECT_EQ(max(c, a), -90);
        EXPECT_EQ(max(c, b), 0);
        EXPECT_EQ(max(b, x), 93);
        EXPECT_EQ(max(x, a), 93);
        EXPECT_EQ(max(x, x), 93);
        EXPECT_EQ(max(x, b), 93);
        EXPECT_EQ(max(c, x), 93);
    }

    bool compare_last_3_bits(int a, int b)
    {
        return (a & 7) < (b & 7);
    }

    TEST(_min_with_custom_comparator)
    {
        int a = -3; //*101
        int b = 9; //*001
        int c = -10000; //*000
        EXPECT_EQ((min<int, compare_last_3_bits>(a, b)), 9);
        EXPECT_EQ((min<int, compare_last_3_bits>(b, a)), 9);
        EXPECT_EQ((min<int, compare_last_3_bits>(a, c)), -10000);
        EXPECT_EQ((min<int, compare_last_3_bits>(c, a)), -10000);
        EXPECT_EQ((min<int, compare_last_3_bits>(b, c)), -10000);
        EXPECT_EQ((min<int, compare_last_3_bits>(c, b)), -10000);
    }

    TEST(_max_with_custom_comparator)
    {
        int a = -3; //*101
        int b = 9; //*001
        int c = -10000; //*000
        EXPECT_EQ((max<int, compare_last_3_bits>(a, b)), -3);
        EXPECT_EQ((max<int, compare_last_3_bits>(b, a)), -3);
        EXPECT_EQ((max<int, compare_last_3_bits>(a, c)), -3);
        EXPECT_EQ((max<int, compare_last_3_bits>(c, a)), -3);
        EXPECT_EQ((max<int, compare_last_3_bits>(b, c)), 9);
        EXPECT_EQ((max<int, compare_last_3_bits>(c, b)), 9);
    }

    TEST(to2)
    {
        EXPECT_EQ(to2(-1), 0);
        EXPECT_EQ(to2(1), 1);
        EXPECT_EQ(to2(10), 16);
        EXPECT_EQ(to2<unsigned char>(128), 128);
        EXPECT_EQ(to2<unsigned char>(129), 0);
        EXPECT_EQ(to2<long long>(0), 1);
    }

    TEST(max_bit_pos)
    {
        EXPECT_EQ(max_bit_pos(0), 255);
        EXPECT_EQ(max_bit_pos(1), 0);
        EXPECT_EQ(max_bit_pos(2), 1);
        EXPECT_EQ(max_bit_pos(3), 1);
        EXPECT_EQ(max_bit_pos(128), 7);
        EXPECT_EQ(max_bit_pos<int>(-1), type_info<int>::bit_length - 1,
                  "Expected %hhu, but %hhu received", type_info<int>::bit_length - 1, max_bit_pos<int>(-1));
        EXPECT_EQ(max_bit_pos<signed char>(-128), type_info<signed char>::bit_length - 1);
        EXPECT_EQ(max_bit_pos<long long>(-128), type_info<long long>::bit_length - 1);
    }

    TEST(min_bit_pos)
    {
        EXPECT_EQ(min_bit_pos(0), 255);
        EXPECT_EQ(min_bit_pos(1), 0);
        EXPECT_EQ(min_bit_pos(2), 1);
        EXPECT_EQ(min_bit_pos(3), 0);
        EXPECT_EQ(min_bit_pos(128), 7);
        EXPECT_EQ(min_bit_pos<int>(-1), 0);
        EXPECT_EQ(min_bit_pos<signed char>(-128), type_info<signed char>::bit_length - 1);
        EXPECT_EQ(min_bit_pos<long long>(-128), 7);
    }

    TEST(pwr)
    {
        int ii = 1, iii = 1;
        unsigned v = 1;
        short l = 1;
        unsigned char iiv = 1;
        long long vii = 1;
        signed char ix = 1;
        for(unsigned i = 0;i < 1000;i++)
        {
            EXPECT_EQ(pow(0, i), i == 0 ? 1 : 0);
            EXPECT_EQ(pow(1, i), 1);
            EXPECT_EQ(pow<int>(2, i), ii);
            EXPECT_EQ(pow<int>(3, i), iii);
            EXPECT_EQ(pow<unsigned>(5, i), v);
            EXPECT_EQ(pow<short>(100, i), l);
            EXPECT_EQ(pow<unsigned char>(3, i), iiv);
            EXPECT_EQ(pow<long long>(7, i), vii);
            EXPECT_EQ(pow<signed char>(9, i), ix);
            ii *= 2, iii *= 3, v *= 5, l *= 100, iiv *= 3, vii *= 7, ix *= 9;
        }
    }
}
