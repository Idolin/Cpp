#include "../debug/test_def.h"
#include "int128.h"

#include <limits>

TEST_PACK(int128)
{
#ifdef E__SIZEOF_INT128__
    TEST(supported_by_platform)
    {
        EXPECT_TRUE(true);
    }

#else
    const uint64_t max64 = std::numeric_limits<uint64_t>::max();

    TEST(uint128_assign)
    {
        uint128_t d = 1;
        EXPECT_EQ(d, 1);
        uint128_t r(123, 567);
        EXPECT_EQ(r, r);
        uint128_t g = r;
        g++;
        EXPECT_NE(g, r);
        g = d;
        EXPECT_EQ(g, 1);
        EXPECT_EQ(g, d);
        uint128_t t;
        t >>= 64;
        EXPECT_NE(t, r);
        EXPECT_NE(t, g);
    }

    TEST(uint128_comparisons)
    {
        SUBTEST(equality)
        {
            uint128_t l = 3;
            uint128_t copy = l;
            uint128_t n = 3;
            uint128_t f = 4;
            uint128_t d = uint128_t(1, 3);
            uint128_t z = 0;
            uint128_t q = uint128_t(1, 2);
            uint128_t r = uint128_t(2, 0);
            uint128_t p = uint128_t(1, 2);
            EXPECT_EQ(l, 3);
            EXPECT_EQ(l, copy);
            EXPECT_EQ(l, n);
            EXPECT_NE(l, 2);
            EXPECT_NE(l, q);
            EXPECT_NE(l, f);
            EXPECT_NE(l, d);
            EXPECT_EQ(q, p);
            EXPECT_NE(d, q);
            EXPECT_NE(r, z);
            EXPECT_EQ(z, 0);
            EXPECT_NE(q, 2);
            EXPECT_NE(r, 0);
        };
        SUBTEST(comparisons)
        {
            uint128_t l = 3;
            uint128_t copy = l;
            uint128_t f = 4;
            uint128_t d = uint128_t(1, 3);
            uint128_t z = 0;
            uint128_t q = uint128_t(1, 2);
            uint128_t r = uint128_t(2, 0);
            uint128_t p = uint128_t(1, 2);
            EXPECT_FALSE(l < 3);
            EXPECT_FALSE(l > 3);
            EXPECT_TRUE(l <= 3);
            EXPECT_TRUE(l >= 3);
            EXPECT_FALSE(l < copy);
            EXPECT_FALSE(l > copy);
            EXPECT_TRUE(l <= copy);
            EXPECT_TRUE(l >= copy);
            EXPECT_GT(l, 2);
            EXPECT_GE(l, 2);
            EXPECT_FALSE(l < 2);
            EXPECT_FALSE(l <= 2);
            EXPECT_LT(l, q);
            EXPECT_LE(l, q);
            EXPECT_GT(q, l);
            EXPECT_GE(q, l);
            EXPECT_LT(l, f);
            EXPECT_LE(l, f);
            EXPECT_FALSE(f <= l);
            EXPECT_LT(l, d);
            EXPECT_LE(l, d);
            EXPECT_GT(d, l);
            EXPECT_GE(d, l);
            EXPECT_GE(q, p);
            EXPECT_LE(p, q);
            EXPECT_LE(q, p);
            EXPECT_GT(d, q);
            EXPECT_GE(d, q);
            EXPECT_LE(q, d);
            EXPECT_LT(q, d);
            EXPECT_GT(r, z);
            EXPECT_GE(r, z);
            EXPECT_GE(z, 0);
            EXPECT_LE(z, 0);
        };
    }

    TEST(uint128_plus)
    {
        uint128_t m(9, 90);
        EXPECT_EQ(m, +m);
        uint128_t z = 0;
        EXPECT_EQ(+z, z);
    }

    TEST(uint128_minus)
    {
        uint128_t z(0);
        uint128_t mx(max64, max64);
        EXPECT_EQ(-z, -z);
        EXPECT_EQ(-z, 0);
        uint128_t o(1);
        EXPECT_EQ(-o, mx);
        EXPECT_EQ(-mx, o);
    }

    TEST(uint128_increment)
    {
        uint128_t z(0);
        EXPECT_EQ(z++, 0);
        EXPECT_EQ(z, 1);
        EXPECT_EQ(++z, 2);
        EXPECT_EQ(z, 2);
        uint128_t m(0, max64);
        m++;
        EXPECT_EQ(m, uint128_t(1, 0));
        ++m;
        EXPECT_EQ(m, uint128_t(1, 1));
        m = uint128_t(max64, max64);
        EXPECT_EQ(++m, 0);
    }

    TEST(uint128_decrement)
    {
        uint128_t z(0);
        uint128_t mx(max64, max64);
        EXPECT_EQ(z--, 0);
        EXPECT_EQ(z, mx);
        z = uint128_t(3, 2);
        EXPECT_EQ(--z, uint128_t(3, 1));
        --z;
        EXPECT_EQ(z, uint128_t(3, 0));
        EXPECT_EQ(z--, uint128_t(3, 0));
        EXPECT_EQ(z, uint128_t(2, max64));
    }

    TEST(uint128_add)
    {
        uint128_t z(0);
        EXPECT_EQ(z, z + z);
        EXPECT_EQ(z + 1, 1);
        EXPECT_EQ(z, 0);
        z += 50;
        EXPECT_EQ(z, 50);
        z += z;
        EXPECT_EQ(z, 100);
        uint128_t o = z;
        z += o;
        EXPECT_EQ(z, 200);
        z = z + o;
        EXPECT_EQ(z, 300);
        z += max64;
        EXPECT_EQ(z, uint128_t(1, 299));
        z += 1;
        EXPECT_EQ(z, uint128_t(1, 300));
        z += z;
        EXPECT_EQ(z, uint128_t(2, 600));
        z = UINT64_C(1) << 63;
        z += z;
        EXPECT_EQ(z, uint128_t(1, 0));
        z = uint128_t(max64, max64);
        z += z;
        EXPECT_EQ(z, uint128_t(max64, max64 - 1));
        z = z + 1;
        EXPECT_EQ(z, uint128_t(max64, max64));
        z += 1;
        EXPECT_EQ(z, 0);
    }

    TEST(uint128_sub)
    {
        uint128_t z(0);
        z -= 0;
        EXPECT_EQ(z, 0);
#ifdef __clang__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wself-assign-overloaded" // see https://bugs.llvm.org/show_bug.cgi?id=43124
#endif
        z -= z;
#ifdef __clang__
    #pragma GCC diagnostic pop
#endif
        EXPECT_EQ(z, 0);
        z = z - 1;
        EXPECT_EQ(z, uint128_t(max64, max64));
        uint128_t h = uint128_t(2, 2);
        h -= 1;
        EXPECT_EQ(h, uint128_t(2, 1));
        h -= 2;
        EXPECT_EQ(h, uint128_t(1, max64));
        h -= max64;
        EXPECT_EQ(h, uint128_t(1, 0));
        h -= max64;
        EXPECT_EQ(h, 1);
        h -= max64;
        EXPECT_EQ(h, uint128_t(max64, 2));
        uint128_t r = h - 7917;
        EXPECT_EQ(h - r, 7917);
        r -= max64;
        EXPECT_EQ(h - r, uint128_t(1, 7916));
    }

    TEST(uint128_shift_left)
    {
        uint128_t z = 0;
        EXPECT_EQ(z << 0, 0);
        EXPECT_EQ(z << 4, 0);
        z++;
        EXPECT_EQ(z << 0, 1);
        EXPECT_EQ(z << 1, 2);
        EXPECT_EQ(z << 63, UINT64_C(1) << 63);
        z = max64;
        EXPECT_EQ(z << 1, uint128_t(1, max64 << 1));
        EXPECT_EQ(z << 10, uint128_t((UINT64_C(1) << 10) - 1, max64 << 10));
        EXPECT_EQ(z << 64, uint128_t(max64, 0));
        EXPECT_EQ(z << 65, uint128_t(max64 - 1, 0));
        EXPECT_EQ(z << 127, uint128_t(max64 << 63, 0));
    }

    TEST(uint128_shift_right)
    {
        uint128_t z = 0;
        EXPECT_EQ(z >> 0, 0);
        EXPECT_EQ(z >> 4, 0);
        z++;
        EXPECT_EQ(z >> 0, 1);
        EXPECT_EQ(z >> 1, 0);
        EXPECT_EQ(z >> 63, 0);
        EXPECT_EQ(z >> 64, 0);
        EXPECT_EQ(z >> 65, 0);
        z = max64;
        EXPECT_EQ(z >> 64, 0);
        EXPECT_EQ(z >> 63, 1);
        z = uint128_t(5, max64);
        EXPECT_EQ(z >> 0, z);
        EXPECT_EQ(z >> 1, uint128_t(2, max64));
        EXPECT_EQ(z >> 2, uint128_t(1, max64 >> 1));
        EXPECT_EQ(z >> 3, max64 ^ (UINT64_C(1) << 62));
        EXPECT_EQ(z >> 63, 11);
        EXPECT_EQ(z >> 64, 5);
        EXPECT_EQ(z >> 65, 2);
        EXPECT_EQ(z >> 66, 1);
        EXPECT_EQ(z >> 67, 0);
        EXPECT_EQ(z >> 127, 0);
        EXPECT_EQ(uint128_t(max64, 9) >> 127, 1);
    }

    TEST(uint128_not)
    {
        uint128_t z = 0;
        EXPECT_EQ(~z, uint128_t(max64, max64));
        z = ~z;
        EXPECT_EQ(~z, 0);
        z = max64;
        EXPECT_EQ(~z, uint128_t(max64, 0));
    }

    TEST(uint128_or)
    {
        uint128_t t = 3;
        uint128_t ht(3, 0);
        uint128_t s(7, 7);
        uint128_t f(5, 0);
        uint128_t lf(5);
        uint128_t mx(max64, max64);
        EXPECT_EQ(t | t, t);
        EXPECT_EQ(s | s, s);
        EXPECT_EQ(s | 0, s);
        EXPECT_EQ(s | t, s);
        EXPECT_EQ(s | f, s);
        EXPECT_EQ(f | lf, uint128_t(5, 5));
        EXPECT_EQ(f | ht, uint128_t(7, 0));
        EXPECT_EQ(mx | 0, mx);
        EXPECT_EQ(mx | f, mx);
    }

    TEST(uint128_and)
    {
        uint128_t t = 3;
        uint128_t ht(3, 0);
        uint128_t s(7, 7);
        uint128_t f(5, 0);
        uint128_t lf(5);
        uint128_t mx(max64, max64);
        EXPECT_EQ(t & t, t);
        EXPECT_EQ(s & s, s);
        EXPECT_EQ(s & 0, 0);
        EXPECT_EQ(s & t, 3);
        EXPECT_EQ(s & f, f);
        EXPECT_EQ(f & lf, 0);
        EXPECT_EQ(f & ht, uint128_t(1, 0));
        EXPECT_EQ(mx & 0, 0);
        EXPECT_EQ(mx & f, f);
    }

    TEST(uint128_xor)
    {
        uint128_t t = 3;
        uint128_t ht(3, 0);
        uint128_t s(7, 7);
        uint128_t f(5, 0);
        uint128_t lf(5);
        uint128_t mx(max64, max64);
        EXPECT_EQ(t ^ t, 0);
        EXPECT_EQ(s ^ s, 0);
        EXPECT_EQ(s ^ 0, s);
        EXPECT_EQ(s ^ t, uint128_t(7, 4));
        EXPECT_EQ(s ^ f, uint128_t(2, 7));
        EXPECT_EQ(f ^ lf, uint128_t(5, 5));
        EXPECT_EQ(f ^ ht, uint128_t(6, 0));
        EXPECT_EQ(mx ^ 0, mx);
        EXPECT_EQ(mx ^ f, uint128_t(max64 ^ 5, max64));
    }

    TEST(uint128_mult)
    {
        uint128_t zero = 0;
        uint128_t one = 1;
        uint128_t two = 2;
        uint64_t rand_low1_64 = 90372016492057295;
        uint64_t rand_low2_64 = 25801470529375221;
        uint128_t rand_low1 = rand_low1_64;
        uint128_t rand_low2 = rand_low2_64;
        uint128_t rand_hi(192837469140, 92147092147);
        uint128_t max_value(max64, max64);

        EXPECT_EQ(zero * zero, 0);
        EXPECT_EQ(rand_low1 * zero, 0);
        EXPECT_EQ(zero * rand_low2, 0);
        EXPECT_EQ(rand_hi * zero, 0);
        EXPECT_EQ(zero * rand_hi, 0);
        EXPECT_EQ(max_value * zero, 0);
        EXPECT_EQ(zero * max_value, 0);

        EXPECT_EQ(one * one, 1);
        EXPECT_EQ(rand_low1 * one, rand_low1);
        EXPECT_EQ(one * rand_low2, rand_low2);
        EXPECT_EQ(rand_hi * one, rand_hi);
        EXPECT_EQ(one * rand_hi, rand_hi);
        EXPECT_EQ(max_value * one, max_value);
        EXPECT_EQ(one * max_value, max_value);

        EXPECT_EQ(two * two, 4);
        EXPECT_EQ(rand_low1 * two, rand_low1 * 2);
        EXPECT_EQ(two * rand_low2, rand_low2 * 2);
        EXPECT_EQ(rand_hi * two, uint128_t(385674938280, 184294184294));
        EXPECT_EQ(two * rand_hi, uint128_t(385674938280, 184294184294));
        EXPECT_EQ(max_value * two, max_value - 1);
        EXPECT_EQ(two * max_value, max_value - 1);

        EXPECT_EQ(rand_low1 * rand_low1, uint128_t(442739452133479, 8273585589890564961));
        EXPECT_EQ(rand_low1 * rand_low2, uint128_t(126403386466624, 13326467307696022811LLU));
        EXPECT_EQ(rand_hi * rand_low1, uint128_t(2600082999288929552, 2715789404866297021));
        EXPECT_EQ(rand_low1 * rand_hi, uint128_t(2600082999288929552, 2715789404866297021));
        EXPECT_EQ(max_value * rand_low1, uint128_t(18446744073709551615LLU, 18356372057217494321LLU));
        EXPECT_EQ(rand_low1 * max_value, uint128_t(18446744073709551615LLU, 18356372057217494321LLU));

        EXPECT_EQ(rand_low2 * rand_low2, uint128_t(36088530247839, 6077723198565841017));
        EXPECT_EQ(rand_hi * rand_low2, uint128_t(6896143691887127437, 13066660453978664527LLU));
        EXPECT_EQ(rand_low2 * rand_hi, uint128_t(6896143691887127437, 13066660453978664527LLU));
        EXPECT_EQ(max_value * rand_low2, uint128_t(18446744073709551615LLU, 18420942603180176395LLU));
        EXPECT_EQ(rand_low2 * max_value, uint128_t(18446744073709551615LLU, 18420942603180176395LLU));

        EXPECT_EQ(rand_hi * rand_hi, uint128_t(10394990511101275204LLU, 5584317241315326249));
        EXPECT_EQ(max_value * rand_hi, uint128_t(18446743880872082475LLU, 18446743981562459469LLU));
        EXPECT_EQ(rand_hi * max_value, uint128_t(18446743880872082475LLU, 18446743981562459469LLU));

        EXPECT_EQ(max_value * max_value, 1);
    }

    TEST(uint128_div)
    {
        uint128_t one = 1;
        uint128_t n127 = 127;
        uint128_t rand32 = 4080666071;
        uint128_t rand64 = 10809554499563826557LLU;

        uint128_t n_2p64(1, 0);
        uint128_t n_2p65m1(1, max64);
        uint128_t rand80bit(44578, 7823781434359621431LLU);
        uint128_t rand96bit(2722184634, 11670191562193564474LLU);
        uint128_t rand112bit(169677796242879, 461613256107031483LLU);
        uint128_t rand128bit(10264771220103218020LLU, 5145448107829437690LLU);
        uint128_t max_value(max64, max64);

        EXPECT_EQ(one / one, 1);
        EXPECT_EQ(n127 / 4, 31);
        EXPECT_EQ(rand32 / 127, 32131228);
        EXPECT_EQ(rand64 / 1, rand64);
        EXPECT_EQ(rand64 / rand32, 10809554499563826557LLU / 4080666071);

        EXPECT_EQ(n_2p64 / n_2p64, 1);
        EXPECT_EQ(n_2p64 / n_2p65m1, 0);
        EXPECT_EQ(n_2p64 / one, n_2p64);
        EXPECT_EQ(n_2p64 / 127, 145249953336295682);
        EXPECT_EQ(n_2p64 / rand32, 4520522814);
        EXPECT_EQ(n_2p64 / rand64, 1);

        EXPECT_EQ(n_2p65m1 / n_2p64, 1);
        EXPECT_EQ(n_2p65m1 / rand64, 3);
        EXPECT_EQ(n_2p65m1 / rand32, 9041045629);
        EXPECT_EQ(n_2p65m1 / 127, 290499906672591364);
        EXPECT_EQ(n_2p65m1 / 17, 2170205185142300190);
        EXPECT_EQ(n_2p65m1 / 4, 9223372036854775807);

        EXPECT_EQ(rand80bit / n_2p65m1, 22289);
        EXPECT_EQ(rand80bit / n_2p64, 44578);
        EXPECT_EQ(rand80bit / rand64, 76074);
        EXPECT_EQ(rand80bit / rand32, 201517783320540);
        EXPECT_EQ(rand80bit / 128, uint128_t(348, 4961039687035034190));
        EXPECT_EQ(rand80bit / 3, uint128_t(14859, 8756841836023057682));

        EXPECT_EQ(rand96bit / rand96bit, 1);
        EXPECT_EQ(rand96bit / rand80bit, 61065);
        EXPECT_EQ(rand96bit / n_2p65m1, 1361092317);
        EXPECT_EQ(rand96bit / rand64, 4645468347);
        EXPECT_EQ(rand96bit / rand32, 12305697747070786069LLU);
        EXPECT_EQ(rand96bit / 7, uint128_t(388883519, 4302419376557588012));

        EXPECT_EQ(rand112bit / rand96bit, 62331);
        EXPECT_EQ(rand112bit / rand80bit, 3806276232);
        EXPECT_EQ(rand112bit / n_2p64, 169677796242879);
        EXPECT_EQ(rand112bit / rand64, 289558915902567);
        EXPECT_EQ(rand112bit / 127, uint128_t(1336045639707, 13076130550314698243LLU));
        EXPECT_EQ(rand112bit / 1, rand112bit);

        EXPECT_EQ(rand128bit / (rand128bit + 1), 0);
        EXPECT_EQ(rand128bit / rand112bit, 60495);
        EXPECT_EQ(rand128bit / rand96bit, 3770784350);
        EXPECT_EQ(rand128bit / rand80bit, 230263214119458);
        EXPECT_EQ(rand128bit / n_2p65m1, 5132385610051609010);
        EXPECT_EQ(rand128bit / rand32, uint128_t(2515464642, 12176735384587544301LLU));

        EXPECT_EQ(max_value / max_value, 1);
        EXPECT_EQ(max_value / rand128bit, 1);
        EXPECT_EQ(max_value / rand96bit, 6776448532);
        EXPECT_EQ(max_value / rand32, uint128_t(4520522814, 15542948953802717536LLU));
        EXPECT_EQ(max_value / 3, uint128_t(6148914691236517205, 6148914691236517205));
        EXPECT_EQ(max_value / 1, max_value);

        EXPECT_EQ((rand96bit * 2 + 3) / rand96bit, 2);
    }

    TEST(uint128_mod)
    {
        uint128_t one = 1;
        uint128_t n127 = 127;
        uint128_t rand32 = 4080666071;
        uint128_t rand64 = 10809554499563826557LLU;

        uint128_t n_2p64(1, 0);
        uint128_t n_2p65m1(1, max64);
        uint128_t rand80bit(44578, 7823781434359621431LLU);
        uint128_t rand96bit(2722184634, 11670191562193564474LLU);
        uint128_t rand112bit(169677796242879, 461613256107031483LLU);
        uint128_t rand128bit(10264771220103218020LLU, 5145448107829437690LLU);
        uint128_t max_value(max64, max64);

        EXPECT_EQ(one % one, 0);
        EXPECT_EQ(n127 % 4, 3);
        EXPECT_EQ(rand32 % 127, 115);
        EXPECT_EQ(rand64 % 1, 0);
        EXPECT_EQ(rand64 % rand32, 10809554499563826557LLU % 4080666071);

        EXPECT_EQ(n_2p64 % n_2p64, 0);
        EXPECT_EQ(n_2p64 % n_2p65m1, n_2p64);
        EXPECT_EQ(n_2p64 % one, 0);
        EXPECT_EQ(n_2p64 % 127, 2);
        EXPECT_EQ(n_2p64 % rand32, 3438307822);
        EXPECT_EQ(n_2p64 % rand64, 7637189574145725059);

        EXPECT_EQ(n_2p65m1 % n_2p64, 18446744073709551615LLU);
        EXPECT_EQ(n_2p65m1 % rand64, 4464824648727623560);
        EXPECT_EQ(n_2p65m1 % rand32, 2795949572);
        EXPECT_EQ(n_2p65m1 % 127, 3);
        EXPECT_EQ(n_2p65m1 % 17, 1);
        EXPECT_EQ(n_2p65m1 % 4, 3);

        EXPECT_EQ(rand80bit % n_2p65m1, 7823781434359643720);
        EXPECT_EQ(rand80bit % n_2p64, 7823781434359621431);
        EXPECT_EQ(rand80bit % rand64, 732099440210062261);
        EXPECT_EQ(rand80bit % rand32, 1456161139);
        EXPECT_EQ(rand80bit % 128, 55);
        EXPECT_EQ(rand80bit % 3, 1);

        EXPECT_EQ(rand96bit % rand96bit, 0);
        EXPECT_EQ(rand96bit % rand80bit, uint128_t(3165, 4681667395588183243));
        EXPECT_EQ(rand96bit % n_2p65m1, 11670191563554656791LLU);
        EXPECT_EQ(rand96bit % rand64, 3557714773557141739);
        EXPECT_EQ(rand96bit % rand32, 3230868119);
        EXPECT_EQ(rand96bit % 7, 6);

        EXPECT_EQ(rand112bit % rand96bit, uint128_t(1305781591, 14657152831498227933LLU));
        EXPECT_EQ(rand112bit % rand80bit, uint128_t(24299, 12624716970383253635LLU));
        EXPECT_EQ(rand112bit % n_2p64, 461613256107031483);
        EXPECT_EQ(rand112bit % rand64, 8710741946730902128);
        EXPECT_EQ(rand112bit % 127, 62);
        EXPECT_EQ(rand112bit % 1, 0);

        EXPECT_EQ(rand128bit % (rand128bit + 1), rand128bit);
        EXPECT_EQ(rand128bit % rand112bit, uint128_t(112936390251401, 8222047509221020229));
        EXPECT_EQ(rand128bit % rand96bit, uint128_t(2019982239, 4751769281079341486));
        EXPECT_EQ(rand128bit % rand80bit, uint128_t(6218, 778724625430567340));
        EXPECT_EQ(rand128bit % n_2p65m1, 10277833717881046700LLU);
        EXPECT_EQ(rand128bit % rand32, 2061030127);

        EXPECT_EQ(max_value % max_value, 0);
        EXPECT_EQ(max_value % rand128bit, uint128_t(8181972853606333595, 13301295965880113925LLU));
        EXPECT_EQ(max_value % rand96bit, uint128_t(2520225988, 14697478408864785271LLU));
        EXPECT_EQ(max_value % rand32, 653619295);
        EXPECT_EQ(max_value % 3, 0);
        EXPECT_EQ(max_value % 1, 0);

        EXPECT_EQ((rand96bit * 2 + 3) % rand96bit, 3);
    }

    TEST(uint128_to_string)
    {
        EXPECT_EQ(uint128_t(0).to_string(), "0");
        EXPECT_EQ(uint128_t(1).to_string(), "1");
        EXPECT_EQ(uint128_t(UINT64_C(9999999999999999999)).to_string(), "9999999999999999999");
        EXPECT_EQ(uint128_t(UINT64_C(10000000000000000000)).to_string(), "10000000000000000000");
        EXPECT_EQ(uint128_t(UINT64_C(10000000000000000001)).to_string(), "10000000000000000001");
        EXPECT_EQ(uint128_t(max64).to_string(), "18446744073709551615");
        EXPECT_EQ(uint128_t(1, 0).to_string(), "18446744073709551616");
        EXPECT_EQ(uint128_t(1, 1).to_string(), "18446744073709551617");
        EXPECT_EQ(uint128_t(max64, max64).to_string(), "340282366920938463463374607431768211455");
    }
#endif
}
