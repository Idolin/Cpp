#include "../debug/test_def.h"
#include "trie.hpp"

TEST_PACK(trie)
{
    TEST(one_insert)
    {
        trie<char> t;
        t.insert("l");
    }

    TEST(check_insert)
    {
        trie<char> t;
        t.insert("1");
        t.insert("2");
        t.insert("4");
        EXPECT_TRUE(t.findKey("1"));
        EXPECT_TRUE(t.findKey("2"));
        EXPECT_FALSE(t.findKey("3"));
        EXPECT_TRUE(t.findKey("4"));
    }

    TEST(check_insert_line)
    {
        trie<char> t;
        t.insert("123");
        t.insert("000");
        t.insert("001");
        t.insert("1234");
        t.insert("22");
        EXPECT_TRUE(t.findKey("123"));
        EXPECT_TRUE(t.findKey("1234"));
        EXPECT_TRUE(t.findKey("000"));
        EXPECT_TRUE(t.findKey("001"));
        EXPECT_TRUE(t.findKey("22"));
        EXPECT_FALSE(t.findKey("00"));
        EXPECT_FALSE(t.findKey(""));
        EXPECT_FALSE(t.findKey("2"));
        t.insert("");
        EXPECT_TRUE(t.findKey(""));
    }

    TEST(check_data_unsinged)
    {
        trie<char, unsigned> t;
        t.insert("", 1);
        t.insert("12", 2);
        t.insert("000", 3);
        t.insert("0000000000000000000", 919);
        EXPECT_EQ(*t.findKey(""), 1);
        EXPECT_EQ(*t.findKey("12"), 2);
        EXPECT_FALSE(t.insert("12", 20, false));
        EXPECT_EQ(*t.findKey("12"), 2);
        EXPECT_FALSE(t.insert("12", 20));
        EXPECT_EQ(*t.findKey("12"), 2);
    }

    TEST(check_erase)
    {
        trie<int16_t, char> t;
        short a[] = {0};
        short b[] = {-1};
        short c[] = {1};
        short d[] = {0, 0};
        short u[] = {2};
        t.insert(a, 1, 'a');
        t.insert(b, 1, 'b');
        t.insert(c, 1, 'c');
        EXPECT_FALSE(t.findKey(u, 1));
        EXPECT_TRUE(t.findKey(b, 1));
        EXPECT_EQ(*t.findKey(a, 1), 'a');
        EXPECT_EQ(*t.findKey(c, 1), 'c');
        EXPECT_EQ(*t.findKey(b, 1), 'b');
        t.insert(d, 2, 'd', true);
        t.insert(b, 1, 'd', true);
        EXPECT_EQ(*t.findKey(d, 2), *t.findKey(b, 1));
        EXPECT_EQ(*t.erase(a, 1), 'a');
        EXPECT_FALSE(t.findKey(a, 1));
        EXPECT_EQ(*t.findKey(d, 2), 'd');
    }
}