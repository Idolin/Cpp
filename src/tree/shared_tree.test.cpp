#include "../debug/test_def.h"
#include "shared_tree.hpp"

#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;
using std::random_shuffle;

TEST_PACK(shared_tree)
{

    TEST(big_data_test)
    {
        int amount = 1000;
        shared_tree<int> a;
        vector<int> params;
        for(int i = 0; i < amount; ++i)
        {
            params.push_back(i);
        }
        random_shuffle(params.begin(), params.end());
        for(int i = 0; i < amount; ++i)
        {
            a.insert(params[i]);
        }
        while(a.begin() != a.end())
        {
            a.erase(a.begin());
        }
    }

    TEST(copy_test)
    {
        int kol = 0;
        int amount = 200000;
        shared_tree<int> copy_set;
        {
            shared_tree<int> my_set;
            vector<int> params;
            for(int i = 0; i < amount; ++i)
            {
                params.push_back(i);
            }
            random_shuffle(params.begin(), params.end());
            for(int i = 0; i < amount; ++i)
            {
                my_set.insert(params[i]);
            }
            copy_set = my_set;
        }
        while(copy_set.begin() != copy_set.end())
        {
            copy_set.erase(copy_set.begin());
            kol++;
        }
        EXPECT_EQ(kol, amount);
    }

    TEST(insert_erase_test_0)
    {
        shared_tree<int> my_set;
        my_set.insert(40);
        my_set.insert(60);
        my_set.insert(20);
        my_set.insert(33);

        {
            shared_tree<int> copy_set(my_set);
            int kol = 0;
            while(copy_set.begin() != copy_set.end())
            {
                copy_set.erase(copy_set.begin());
                kol++;
            }
            EXPECT_EQ(kol, 4);
        }
        int amount = 0;
        while(my_set.begin() != my_set.end())
        {
            cout << *my_set.begin() << ' ';
            amount++;
            my_set.erase(my_set.begin());
        }
        EXPECT_EQ(amount, 4);
        cout << endl;
    }

    TEST(insert_erase_test)
    {
        shared_tree<int> my_set;
        my_set.insert(40);
        my_set.insert(20);
        my_set.insert(60);
        my_set.insert(10);
        my_set.insert(30);
        my_set.insert(50);
        my_set.insert(70);
        my_set.insert(45);
        my_set.insert(55);
        my_set.insert(43);
        my_set.insert(47);
        my_set.insert(46);
        my_set.insert(48);
        my_set.insert(49);

        my_set.erase(my_set.find(50));
        {
            shared_tree<int> copy_set(my_set);
            int kol = 0;
            while(copy_set.begin() != copy_set.end())
            {
                EXPECT_NE(*copy_set.begin(), 50);
                copy_set.erase(copy_set.begin());
                kol++;
            }
            EXPECT_EQ(kol, 13);
        }

        my_set.erase(my_set.find(49));
        {
            shared_tree<int> copy_set(my_set);
            int kol = 0;
            while(copy_set.begin() != copy_set.end())
            {
                EXPECT_NE(*copy_set.begin(), 49);
                copy_set.erase(copy_set.begin());
                kol++;
            }
            EXPECT_EQ(kol, 12);
        }

        my_set.erase(my_set.find(55));
        {
            shared_tree<int> copy_set(my_set);
            int kol = 0;
            while(copy_set.begin() != copy_set.end())
            {
                EXPECT_NE(*copy_set.begin(), 55);
                copy_set.erase(copy_set.begin());
                kol++;
            }
            EXPECT_EQ(kol, 11);
        }

        my_set.erase(my_set.find(40));
        {
            shared_tree<int> copy_set(my_set);
            int kol = 0;
            while(copy_set.begin() != copy_set.end())
            {
                EXPECT_NE(*copy_set.begin(), 40);
                copy_set.erase(copy_set.begin());
                kol++;
            }
            EXPECT_EQ(kol, 10);
        }
        while(my_set.begin() != my_set.end())
        {
            cout << *my_set.begin() << ' ';
            my_set.erase(my_set.begin());
        }
        cout << endl;
    }

    TEST(iterator_test)
    {
        shared_tree<int> my_set;
        my_set.insert(40);
        my_set.insert(20);
        my_set.insert(60);
        my_set.insert(10);
        my_set.insert(30);
        my_set.insert(50);
        my_set.insert(70);
        my_set.insert(45);
        my_set.insert(55);
        my_set.insert(43);
        my_set.insert(47);
        my_set.insert(46);
        my_set.insert(48);
        my_set.insert(49);

        for(shared_tree<int>::iterator it = my_set.begin(); it != my_set.end(); it++)
        {
            cout << *it << ' ';
        }
        cout << endl;
        for(shared_tree<int>::iterator it = my_set.begin(); it != my_set.end(); ++it)
        {
            cout << *it << ' ';
        }
        cout << "\n10 20 30 40 43 45 46 47 48 49 50 55 60 70 \n\n";
        for(shared_tree<int>::iterator it = --my_set.end(); it != my_set.begin(); --it)
        {
            cout << *it << ' ';
        }
        cout << *my_set.begin() << endl;
        for(shared_tree<int>::iterator it = --my_set.end(); it != my_set.begin(); it--)
        {
            cout << *it << ' ';
        }
        cout << *my_set.begin() << endl;
        cout << "70 60 55 50 49 48 47 46 45 43 40 30 20 10\n";
    }

    TEST(first)
    {
        shared_tree<int> *a = new shared_tree<int>();
        a->insert(4);
        auto it = a->find(4);
        EXPECT_NE(it, a->end());
        a->erase(it);
        EXPECT_EQ(a->find(4), a->end());
        delete a;
    }

    TEST(second)
    {
        shared_tree<int> *a = new shared_tree<int>();
        for(int i = 0; i < 30; i++)
            a->insert(i);
        for(int i = 0; i < 30; i++)
        {
            auto it = a->find(i);
            EXPECT_NE(it, a->end());
            a->erase(it);
        }
        delete a;
    }

    TEST(third)
    {
        shared_tree<unsigned long> *a = new shared_tree<unsigned long>;
        for(int i = 0; i < 50; i++)
            a->insert(i);
        shared_tree<unsigned long> b = shared_tree<unsigned long>(*a);
        for(int k = 0; k < 50; k++)
            a->insert(k + 100);
        for(int i = 0; i < 50; i++)
            EXPECT_NE(b.find(i), b.end());
        for(int k = 0; k < 50; k++)
            EXPECT_EQ(b.find(k + 100), b.end());
        delete a;
    }
};
