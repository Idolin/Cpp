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
        UNUSED(e);
        delete [] c;
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
        str b = a.subStr(3);
        str c = a.subStr(1, 4);
        EXPECT_EQ(b, "345");
        EXPECT_EQ(c, "123");
        EXPECT_EQ(b[0], '3');
        EXPECT_EQ(c[2], b[0]);
    }
}