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
            EXPECT_EQ(q.pop(), i);
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
            for(int k = 0; k < 10; k++)
                EXPECT_EQ(c[k], q.pop());
        }
    }

    TEST(blocksize_1)
    {
        vect<signed char> c;
        queue<signed char> q(1);
        unsigned k = 0;
        for(int i = 0; i < 11000; i++)
            if((k < c.maxs) && (randomC() < 19))
            {
                EXPECT_EQ(q.pop(), c[k++]);
            } else
            {
                c.push(static_cast<signed char>(randomC()));
                q.push(c.back());
            }
        while(k < c.maxs)
            EXPECT_EQ(q.pop(), c[k++]);
    }
};
