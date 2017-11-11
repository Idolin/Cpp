#include "avl.hpp"
#include "../debug/test_def.h"

TEST_PACK(avl_tree, STOP_AFTER_ERROR(1))
{

    TEST(insert_3_and_iterate)
    {
        avl_tree<int> *a = new avl_tree<int>;
        a->insert(0, 0);
        a->insert(2, 2);
        a->insert(1, 1);
        EXPECT_TRUE(check_avl_tree(a), "Tree is not AVL!");
        int expected_number = 0;
        for(auto it = a->begin(); it != a->end(); it++)
            EXPECT_EQ(*it, expected_number++);
        EXPECT_EQ(expected_number, 3);
        delete a;
    }

    TEST(insert_and_remove_1)
    {
        avl_tree<int> *a = new avl_tree<int>;
        EXPECT_TRUE(a->empty());
        a->insert(0, 0);
        EXPECT_TRUE(!a->empty());
        a->erase(a->begin());
        EXPECT_TRUE(a->empty());
        delete a;
    }

    TEST(insert_and_remove_3)
    {
        avl_tree<int> *a = new avl_tree<int>;
        EXPECT_TRUE(a->empty());
        a->insert(2, 2);
        a->insert(1, 1);
        a->insert(0, 0);
        EXPECT_TRUE(check_avl_tree(a), "Tree is not AVL!");
        a->erase(a->begin());
        EXPECT_TRUE(check_avl_tree(a), "Tree is not AVL!");
        a->erase(a->begin());
        EXPECT_TRUE(check_avl_tree(a), "Tree is not AVL!");
        EXPECT_TRUE(!a->empty());
        a->erase(a->begin());
        EXPECT_TRUE(a->empty());
        delete a;
    }

    TEST(insert_and_remove_7, STOP_AFTER_ERROR(1))
    {
        avl_tree<int> *a = new avl_tree<int>;
        a->insert(0, 0);
        a->insert(1, 1);
        a->insert(2, 2);
        a->insert(3, 3);
        a->insert(4, 4);
        a->insert(5, 5);
        auto it6 = a->insert(6, 6).first;
        auto it0 = a->findKey(0);
        auto it1 = a->findKey(1);
        auto it2 = a->findKey(2);
        auto it3 = a->findKey(3);
        auto it4 = a->findKey(4);
        auto it5 = a->findKey(5);
        EXPECT_EQ(*it0, 0);
        EXPECT_EQ(*it1, 1);
        EXPECT_EQ(*it2, 2);
        EXPECT_EQ(*it3, 3);
        EXPECT_EQ(*it4, 4);
        EXPECT_EQ(*it5, 5);
        EXPECT_EQ(*it6, 6);
        EXPECT_EQ(a->erase(it3), 3);
        EXPECT_TRUE(check_avl_tree(a));
        EXPECT_EQ(a->erase(a->findKey(1)), 1);
        EXPECT_TRUE(check_avl_tree(a));
        EXPECT_EQ(a->erase(a->findKey(5)), 5);
        EXPECT_TRUE(check_avl_tree(a));
        EXPECT_EQ(a->erase(a->findKey(0)), 0);
        EXPECT_EQ(a->erase(a->findKey(6)), 6);
        EXPECT_EQ(a->erase(a->findKey(2)), 2);
        EXPECT_EQ(a->erase(a->findKey(4)), 4);
        EXPECT_TRUE(a->empty());
        delete a;
    }

    TEST(insert_if_exists)
    {
        avl_tree<int> *a = new avl_tree<int>;
        a->insert(5, 5);
        auto it = a->insert(5, 6);
        EXPECT_FALSE(it.second);
        EXPECT_EQ(*it.first, 5);
        it = a->insert(5, 7, true);
        EXPECT_FALSE(it.second);
        EXPECT_EQ(*it.first, 7);
        delete a;
    }

    TEST(random)
    {
        avl_tree<int> *a = new avl_tree<int>;
        for(int i = 0; i <= 10000; i++)
            a->insert(i, i * 2);
        for(int i = 10000; i >= 0; i--)
            EXPECT_EQ(a->erase(a->findKey(i)), i * 2);
        EXPECT_TRUE(a->empty());
        delete a;
    }

};
