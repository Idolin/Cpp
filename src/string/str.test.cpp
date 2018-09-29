#include "../debug/test_def.h"
#include "str.h"
#include "../other/rand.h"

TEST_PACK(str)
{
    TEST(create)
    {
        str a("1");
        str b = "Hello, World!";
        str c;
        str d(c);
        str e = d;
    }

    TEST(convert)
    {
        std::string a = "string";
        str b = a;
        char *c = b;
        UNUSED(c);
        const str d = b;
        const char *e = d;
        const char *f = d.c_str_ptr();
        EXPECT_STRING_EQ(e, f);
        delete [] c;
        delete [] e; //convert to char* always creates new array(use c_str instead)
    }

    TEST(compare)
    {
        std::string a = "a";
        std::string b = "";
        str c;
        str d = "b";
        str e = "b";
        str f = d;
        EXPECT_EQ(a, str("a"));
        EXPECT_EQ(b, c);
        EXPECT_NE(a, c);
        EXPECT_NE(a, d);
        EXPECT_NE(c, d);
        EXPECT_EQ(c, str());
        EXPECT_EQ(d, "b");
        EXPECT_EQ(f, d);
        EXPECT_EQ(d, e);
        EXPECT_EQ(f, "b");
        f = "";
        EXPECT_EQ(c, f);
    }

    TEST(access)
    {
        str a = "012345";
        EXPECT_EQ(a[4], '4');
        EXPECT_EQ(a[2], '2');
        EXPECT_EQ(a[0], '0');
        EXPECT_EQ(a[5], '5');
    }

    TEST(substr_access)
    {
        str a = "012345";
        str b = a.subStr(3);
        str c = a.subStr(1, 4);
        EXPECT_EQ(b, "345");
        EXPECT_EQ(c, "123");
        EXPECT_EQ(b[0], '3');
        EXPECT_EQ(c[2], b[0]);
    }

    TEST(concatenation)
    {
        str a = "1";
        str b = "2";
        str c = "3";
        str d = a + b;
        str e = str("1") + "2";
        str f = d;
        EXPECT_EQ(d, e);
        EXPECT_EQ(f, "12");
        EXPECT_EQ(e, f);
        str g = a + b + c;
        str h = d + "3";
        str j = f + c;
        EXPECT_EQ(g, h);
        EXPECT_EQ(g, j);
        EXPECT_EQ(j, "123");
        a = d;
        b = c;
        EXPECT_EQ(d, "12");
        EXPECT_EQ(a, "12");
        EXPECT_EQ(c, b);
        EXPECT_EQ(b, "3");
    }

    TEST(multiplication)
    {
        str _1000 = "0";
        _1000 *= 1000;
        EXPECT_EQ(_1000.length(), 1000);
        EXPECT_EQ(_1000.at(999), '0');
    }

    TEST(concatenation_memory)
    {
        str a = "1";
        str b = "2";
        for(int i = 0;i < 40; i++)
            if(i & 1)
                a += b;
            else
                b += a;
        EXPECT_EQ(b.length(), 165580141); //41th fibonacci number
        EXPECT_EQ(b.at(312), '2');
    }

    TEST(concatenation_speed)
    {
        str a;
        str one = "1";
        str two = "2";
        str three = "3";
        for(int i = 0;i < 10000;i++)
            if(i & 1)
                a += one;
            elif(i & 2)
                a += two;
            else
                a += three;
        EXPECT_EQ(a.length(), 10000);
        EXPECT_EQ(a.at(0), '3');
        EXPECT_EQ(a.at(797), '1');
    }

    TEST(substr)
    {
        str a = "1234567890";
        str b = a.subStr(0, 5);
        EXPECT_EQ(b, "12345");
        str c = a.subStr(5);
        EXPECT_EQ(c, "67890");
        EXPECT_EQ(a, b + c);
        str d = b.subStr(3);
        EXPECT_EQ(d, "45");
        EXPECT_EQ(a.subStr(4), a(4));
        EXPECT_EQ(a.subStr(1, 2), a(1, 2));
        EXPECT_EQ(d(2), "");
        EXPECT_EQ(a(7), a(7, 10));
        EXPECT_EQ(a(1, 7)(1, 2), "3");
        EXPECT_EQ(a(2)(1, 5), "4567");
        str e = a + b + c + d;
        EXPECT_EQ(e(4, 16), "567890123456");
    }

    TEST(not_equals)
    {
        str a = "1";
        str b = "@";
        EXPECT_NE(a, b);
        str c;
        EXPECT_NE(a, c);
        str q = "1";
        q *= 2;
        EXPECT_NE(a, q);
        c = "0";
        c *= 31;
        EXPECT_NE("a" + c, c + "a");
        c += '0';
        EXPECT_NE("a" + c + "b", "b" + c + "a");
    }

    TEST(invert)
    {
        str a = "12323410";
        str b = a.subStr(2).subStr(1, 5);
        EXPECT_EQ(b, "2341");
        b = b.invert();
        EXPECT_EQ(b, "1432");
        EXPECT_EQ(a.invert(), "01432321");
    }

    TEST(write_access)
    {
        str a = "12345";
        a[2] = '0';
        EXPECT_EQ(a, "12045");
        str b = a;
        a[4] = '0';
        EXPECT_EQ(a, "12040");
        EXPECT_EQ(b, "12045");
        str c = a(2, 3);
        EXPECT_EQ(c, "0");
        c[0] = '5';
        EXPECT_EQ(c, "5");
        EXPECT_EQ(a, "12040");
    }

    TEST(write_aceess_through)
    {
        str a = str("1") * 10000;
        str p = a;
        for(int i = 0; i < 9992; i++)
            p = p(1);
        a = "some other string";
        p[0] = '2';
        EXPECT_EQ(p, "21111111");
    }

    TEST(char_append)
    {
        str d = ">";
        for(int i = 0;i < 9;i++)
            d += '^';
        str h;
        h += '>';
        EXPECT_EQ(h, ">", "\"%s\" != \">\"", h.c_str());
        h += "^^^^^^^^^";
        EXPECT_EQ(d, h, "\"%s\" != \"%s\" (expected == \">^^^^^^^^^\" for both)",
            d.c_str(), h.c_str());
        str s = "str";
        str c;
        c += 'c';
        str u;
        unsigned str_count = 0;
        for(unsigned i = 0;i < 100000;i++)
            if(randomUC() < 5)
            {
                u += s;
                str_count++;
            }
            else
                u += c;
        EXPECT_EQ(u.length(), str_count * 2 + 100000,
                  "Expected length of str: %lu, but length is %lu",
                  str_count * 2 + 100000, u.length());
    }

    TEST(last_char_is_nul)
    {
        str e;
        EXPECT_EQ(e.at(0), '\0');
        for(int i = 0;i < 10;i++)
        {
            e += '1';
            EXPECT_EQ(e.at(e.length()), '\0');
        }
        str a = "1234567";
        EXPECT_EQ(a.at(a.length()), '\0');
        e += a;
        EXPECT_EQ(e.at(e.length()), '\0');
        for(int i = 0;i < 10;i++)
        {
            e += '1';
            EXPECT_EQ(e.at(e.length()), '\0');
        }
        a = e.subStr(3, 7);
        EXPECT_EQ(a.at(a.length()), '\0');
        a = a.subStr(3);
        EXPECT_EQ(a.at(a.length()), '\0');
        a = "";
        EXPECT_EQ(a.at(a.length()), '\0');
    }

    TEST(str_comparision)
    {
        str a, b;
        EXPECT_LE(a, b);
        EXPECT_GE(a, b);
        EXPECT_EQ(a, b);
        a = "123";
        EXPECT_LE(b, a);
        EXPECT_FALSE(a <= b);
        b = "12";
        EXPECT_LE(b, a);
        EXPECT_FALSE(a <= b);
        b += '5';
        EXPECT_GT(b, a);
        EXPECT_LE(a, b);
    }

    TEST(str_equals_speed)
    {
        str a = "1";
        str b = "1";
        for(unsigned i = 0;i < 22;i++)
        {
            EXPECT_EQ(a, b);
            a += a;
            b += b;
        }
    }

    TEST(str_comparision_speed)
    {
        for(unsigned i = 0;i < 100;i++)
        {
            str a = "1";
            str b = "1";
            a *= 100000;
            b *= 100000;
            EXPECT_EQ(a, b);
            EXPECT_LE(b, a);
            EXPECT_GE(b, a);
            EXPECT_FALSE(b < a);
            a = "0" + a;
            EXPECT_LT(a, b);
            EXPECT_NE(a, b);
            b = "0" + b + "1";
            EXPECT_LT(a, b);
            EXPECT_NE(a, b);
        }
    }
}
