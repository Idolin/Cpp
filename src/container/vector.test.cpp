#include "../debug/test_def.h"
#include "../other/rand.h"
#include "vector.hpp"

TEST_PACK(vector)
{
    TEST(add_get_elements)
    {
        vect<int> v;
        v.push(0);
        for(int i = 1;i < 10000;i++)
        {
            if(randomUC() < 20)
            {
                unsigned index = randomA<unsigned>(i);
                EXPECT_EQ(v.at(index), index);
            }
            v.push(i);
        }
        EXPECT_EQ(v.at(9999), 9999);
    }
}
