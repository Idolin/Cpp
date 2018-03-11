#include "../debug/test_def.h"
#include "str.h"

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
        const char *f = d.c_str();
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
        EXPECT_EQ(_1000[999], '0');
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
        EXPECT_EQ(b[312], '2');
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
        EXPECT_EQ(a[0], '3');
        EXPECT_EQ(a[797], '1');
    }

    TEST(substr)
    {
        str a = "1234567890";
        str b = a.subStr(0, 5);
        str c = a.subStr(5);
        EXPECT_EQ(a, b + c);
        str d = b.subStr(3);
        EXPECT_EQ(d, "45");
        EXPECT_EQ(a.subStr(4), a(4));
        EXPECT_EQ(a.subStr(1, 2), a(1, 2));
        EXPECT_EQ(d(2), "");
        EXPECT_EQ(a(7), a(7, 10));
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
    }
}