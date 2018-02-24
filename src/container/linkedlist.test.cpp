//
// Created by cynder on 01.01.18.
//

#include "../debug/test_def.h"
#include "linkedlist.hpp"

TEST_PACK(linkedlist_one_direction)
{

    TEST(empty_on_create)
    {
        linkedList<int, false> l;
        EXPECT_TRUE(l.empty());
        EXPECT_EQ(l.len, 0);
    }

    TEST(one_element)
    {
        linkedList<int, false> l;
        l.push_front(3);
        EXPECT_EQ(l.head->data, 3);
        EXPECT_EQ(l.head, l.tail);
        EXPECT_FALSE(l.empty());
        EXPECT_EQ(l.remove_head(), 3);
        EXPECT_TRUE(l.empty());
        EXPECT_EQ(l.head, l.end());
    }

    TEST(three_elements)
    {
        linkedList<unsigned char, false> l;
        l.push_front(2);
        l.push_tail(3);
        l.push_front(1);
        EXPECT_EQ(l.remove_next(l.head), 2);
        EXPECT_EQ(l.len, 2);
        EXPECT_EQ(l.remove_next(l.end()), 1);
        EXPECT_EQ(l.len, 1);
        EXPECT_EQ(l.remove_head(), 3);
        EXPECT_TRUE(l.empty());
    }

    TEST(few_add_remove_iterations)
    {
        linkedList<unsigned, false> l;
        for(unsigned test = 0; test < 3; test++)
        {
            EXPECT_TRUE(l.empty());
            l.push_tail(92);
            EXPECT_EQ(l.remove_head(), 92);
        }
        EXPECT_TRUE(l.empty());
        for(unsigned test = 0; test < 3; test++)
        {
            linkedList<unsigned, false>::iterator middle = l.push_tail(0);
            for(unsigned i = 2; i < 100; i++)
            {
                l.push_tail(i);
                l.push_front(i);
            }
            l.insert(middle, 1);
            for(unsigned i = 1; i < 100; i++)
                EXPECT_EQ(l.remove_next(middle), i);
            for(unsigned i = 99; i > 1; i--)
                EXPECT_EQ(l.remove_head(), i);
            EXPECT_EQ(l.remove_head(), 0);
            EXPECT_TRUE(l.empty());
        }
    }

    TEST(insert)
    {
        linkedList<bool, false> l;
        for(unsigned i = 0; i < 3; i++)
            l.push_front(false);
        linkedList<bool, false>::iterator it = l.push_tail(false);
        for(unsigned i = 0; i < 3; i++)
            l.push_tail(false);
        l.insert(it, true);
        EXPECT_EQ(l.len, 8);
        for(unsigned i = 0; i < 8; i++)
            EXPECT_EQ(l.remove_head(), (i == 4));
        EXPECT_TRUE(l.empty());
    }

    TEST(at)
    {
        linkedList<unsigned, false> l;
        for(unsigned i = 0; i < 300; i++)
            l.push_tail(i);
        EXPECT_EQ(l.at(0)->data, 0);
        EXPECT_EQ(l.at(30)->data, 30);
        EXPECT_EQ(l.at(100)->data, 100);
        EXPECT_EQ(l.at(298)->data, 298);
        EXPECT_EQ(l.at(299)->data, 299);
    }

    TEST(iterator)
    {
        linkedList<unsigned, false> l;
        for(unsigned i = 0; i < 100; i++)
            l.push_tail(i);
        linkedList<unsigned, false>::iterator it = l.begin();
        linkedList<unsigned, false>::iterator it_copy = it;
        unsigned i = 0;
        for(; it != l.end(); it++)
            EXPECT_EQ(it->data, i++);
        EXPECT_EQ(it_copy->data, 0);
        EXPECT_EQ(it_copy++->data, 0);
        EXPECT_EQ((++it_copy)->data, 2);
        i = 0;
        for(unsigned k : l)
            EXPECT_EQ(k, i++);
        EXPECT_EQ((linkedList<unsigned, false>::iterator()), l.end());
    }
};

TEST_PACK(linkedlist_bidirectional)
{

    TEST(empty_on_create)
    {
        linkedList<int> l;
        EXPECT_TRUE(l.empty());
        EXPECT_EQ(l.len, 0);
    }

    TEST(one_element)
    {
        linkedList<int> l;
        l.push_front(3);
        EXPECT_EQ(l.head->data, 3);
        EXPECT_EQ(l.head, l.tail);
        EXPECT_FALSE(l.empty());
        EXPECT_EQ(l.remove_head(), 3);
        EXPECT_TRUE(l.empty());
        EXPECT_EQ(l.head, l.end());
    }

    TEST(three_elements)
    {
        linkedList<unsigned char> l;
        l.push_front(2);
        l.push_tail(3);
        l.push_front(1);
        EXPECT_EQ(l.remove(++l.begin()), 2);
        EXPECT_EQ(l.len, 2);
        EXPECT_EQ(l.remove_tail(), 3);
        EXPECT_EQ(l.len, 1);
        EXPECT_EQ(l.remove_head(), 1);
        EXPECT_TRUE(l.empty());
    }

    TEST(few_add_remove_iterations)
    {
        linkedList<unsigned> l;
        for(unsigned test = 0; test < 3; test++)
        {
            EXPECT_TRUE(l.empty());
            l.push_tail(92);
            EXPECT_EQ(l.remove_head(), 92);
        }
        EXPECT_TRUE(l.empty());
        for(unsigned test = 0; test < 3; test++)
        {
            EXPECT_TRUE(l.empty());
            linkedList<unsigned>::iterator middle = l.push_tail(0);
            for(unsigned i = 2; i < 100; i++)
            {
                l.push_tail(i);
                l.push_front(i);
            }
            l.insert(middle, 1);
            for(unsigned i = 1; i < 100; i++)
                EXPECT_EQ(l.remove(middle->next), i);
            for(unsigned i = 2; i < 100; i++)
                EXPECT_EQ(l.remove(middle->prev), i);
            EXPECT_EQ(l.remove_head(), 0);
        }
        EXPECT_TRUE(l.empty());
    }

    TEST(insert)
    {
        linkedList<bool> l;
        for(unsigned i = 0; i < 3; i++)
            l.push_front(false);
        linkedList<bool>::iterator it = l.push_tail(false);
        for(unsigned i = 0; i < 3; i++)
            l.push_tail(false);
        l.insert(it, true);
        EXPECT_EQ(l.len, 8);
        for(unsigned i = 0; i < 8; i++)
            EXPECT_EQ(l.remove_head(), (i == 4));
        EXPECT_TRUE(l.empty());
    }

    TEST(at)
    {
        linkedList<unsigned> l;
        for(unsigned i = 0; i < 300; i++)
            l.push_tail(i);
        EXPECT_EQ(l.at(0)->data, 0);
        EXPECT_EQ(l.at(30)->data, 30);
        EXPECT_EQ(l.at(100)->data, 100);
        EXPECT_EQ(l.at(298)->data, 298);
        EXPECT_EQ(l.at(299)->data, 299);
    }

    TEST(iterator)
    {
        linkedList<unsigned> l;
        for(unsigned i = 0; i < 100; i++)
            l.push_tail(i);
        linkedList<unsigned>::iterator it = l.begin();
        linkedList<unsigned>::iterator it_copy = it;
        unsigned i = 0;
        for(; it != l.end(); it++)
            EXPECT_EQ(it->data, i++);
        EXPECT_EQ(it_copy->data, 0);
        EXPECT_EQ(it_copy++->data, 0);
        EXPECT_EQ((++it_copy)->data, 2);
        EXPECT_EQ((--l.end())->data, 99);
        EXPECT_EQ((--it_copy)->data, 1);
        EXPECT_EQ(it_copy--->data, 1);
        i = 0;
        for(unsigned k : l)
            EXPECT_EQ(k, i++);
        EXPECT_EQ(linkedList<unsigned>::iterator(), l.end());
    }
};