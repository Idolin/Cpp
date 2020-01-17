#include "../other/rand.h"
#include "../debug/test_def.h"
#include "vector.hpp"
#include "queue.hpp"

TEST_PACK(queue)
{
    TEST(int_1)
    {
        queue<int> q;
        for(int i = -10; i < 10000; i++)
        {
            q.push(i);
			EXPECT_FALSE(q.empty());
            EXPECT_EQ(q.pop(), i);
			EXPECT_TRUE(q.empty());
        }
    }

    TEST(long_10)
    {
        vect<long> c;
        queue<long> q;
        for(int i = 0; i < 10000; i++)
        {
            for(int k = 0; k < 10; k++)
            {
                c[k] = randomL();
                q.push(c[k]);
            }
            EXPECT_EQ(q.size(), 10);
            for(int k = 0; k < 10; k++)
                EXPECT_EQ(c[k], q.pop());
			EXPECT_EQ(q.size(), 0);
        }
    }

    TEST(blocksize_1)
    {
        vect<signed char> c;
        queue<signed char> q(1);
        unsigned k = 0;
        for(int i = 0; i < 11000; i++)
            if((k < c.size()) && (randomC() < 19))
            {
				EXPECT_FALSE(q.empty());
                EXPECT_EQ(q.pop(), c[k++]);
            }
            else
            {
                c.push(static_cast<signed char>(randomC()));
                q.push(c.back());
            }
        while(k < c.size())
            EXPECT_EQ(q.pop(), c[k++]);
		EXPECT_TRUE(q.empty());
    }

    TEST(speed_10000000)
    {
        queue<unsigned long long> q;
        unsigned long k = 0;
        for(unsigned long i = 0;i < 10000000;)
            if(k < i && randomUC() < 79)
                EXPECT_EQ(k++, q.pop())
            else
                q.push(i++);
        for(;k < 10000000;k++)
            EXPECT_EQ(q.pop(), k);
		EXPECT_TRUE(q.empty());
    }

    TEST(speed_periodically_clear)
    {
        queue<unsigned long long> q;
        unsigned long long k = 0;
        for(unsigned long long i = 0;i < 10000000;)
        {
            if(randomUC() == 73)
                while(!q.empty())
                    EXPECT_EQ(k++, q.pop());
            q.push(i++);
        }
        while(!q.empty())
            EXPECT_EQ(k++, q.pop());
    }
};

TEST_PACK(queue_cycled)
{
    TEST(int_1)
    {
        queue_cycled<int> q;
        for(int i = -10; i < 10000; i++)
        {
            q.push(i);
			EXPECT_FALSE(q.empty());
            EXPECT_EQ(q.pop(), i);
			EXPECT_TRUE(q.empty());
        }
    }

    TEST(long_10)
    {
        vect<long> c;
        queue_cycled<long> q;
        for(int i = 0; i < 10000; i++)
        {
            for(int k = 0; k < 10; k++)
            {
                c[k] = randomL();
                q.push(c[k]);
            }
            EXPECT_EQ(q.size(), 10);
            for(int k = 0; k < 10; k++)
                EXPECT_EQ(c[k], q.pop());
			EXPECT_EQ(q.size(), 0);
        }
    }

    TEST(blocksize_1)
    {
        vect<signed char> c;
        queue_cycled<signed char> q(1);
        unsigned k = 0;
        for(int i = 0; i < 11000; i++)
            if((k < c.size()) && (randomC() < 19))
            {
				EXPECT_FALSE(q.empty());
                EXPECT_EQ(q.pop(), c[k++]);
            }
            else
            {
                c.push(static_cast<signed char>(randomC()));
                q.push(c.back());
            }
        while(k < c.size())
            EXPECT_EQ(q.pop(), c[k++]);
		EXPECT_TRUE(q.empty());
    }

    TEST(speed_10000000)
    {
        queue_cycled<unsigned long long> q;
        unsigned long k = 0;
        for(unsigned long i = 0;i < 10000000;)
            if(k < i && randomUC() < 79)
                EXPECT_EQ(k++, q.pop())
            else
                q.push(i++);
        for(;k < 10000000;k++)
            EXPECT_EQ(q.pop(), k);
		EXPECT_TRUE(q.empty());
    }

    TEST(speed_periodically_clear)
    {
        queue_cycled<unsigned long long> q;
        unsigned long long k = 0;
        for(unsigned long long i = 0;i < 10000000;)
        {
            if(randomUC() == 73)
                while(!q.empty())
                    EXPECT_EQ(k++, q.pop());
            q.push(i++);
        }
        while(!q.empty())
            EXPECT_EQ(k++, q.pop());
    }
};
