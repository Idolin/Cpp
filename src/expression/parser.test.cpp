#include "parser.h"
#include "../debug/test_def.h"

TEST_PACK(parser)
{

    TEST(parse_one_value)
    {
        Expression a = parse("5");
        EXPECT_EQ(a(0), 5);
        EXPECT_EQ(a(5), 5);
    }

    TEST(parse_binary_oprations)
    {
        Expression a = parse("5+x");
        //Expression b = parse("x-19");
        //Expression c = parse("x*x");
        //Expression d = parse("x/2");
        //Expression e = parse("x^1.5");
        EXPECT_DOUBLE_EQ(a(0), 5);
        EXPECT_DOUBLE_EQ(a(-5), 0);
        //EXPECT_DOUBLE_EQ(b(3), -16);
        //EXPECT_DOUBLE_EQ(b(21), 2);
        //EXPECT_DOUBLE_EQ(c(3), 9);
        //EXPECT_DOUBLE_EQ(c(-1), 1);
        //EXPECT_DOUBLE_EQ(d(5), 2.5);
        //EXPECT_DOUBLE_EQ(d(220), 110);
        //EXPECT_DOUBLE_EQ(e(1), 1);
        //EXPECT_DOUBLE_EQ(e(4), 8);
    }

};
