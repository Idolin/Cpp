//
// Created by cynder on 21.06.19.
//

#include "bit_array.h"
#include "../debug/test_def.h"
#include "../other/rand.h"

TEST_PACK(bit_array)
{
    TEST(8bit)
    {
        bit_array b(8);

        EXPECT_EQ(b.size(), 8);

        EXPECT_FALSE(b[0]);
        EXPECT_FALSE(b[7]);

        b.set(2);
        b.set(3, true);
        b.clear(4);
        b.set(6, false);

        EXPECT_FALSE(b[1]);
        EXPECT_TRUE(b[2]);
        EXPECT_TRUE(b[3]);
        EXPECT_FALSE(b[4]);
        EXPECT_FALSE(b[6]);

        b.clear(3);
        b.set(6, true);

        EXPECT_FALSE(b.get(3));
        EXPECT_TRUE(b.get(6));

        b.clear();

        EXPECT_FALSE(b.get(2));
        EXPECT_FALSE(b[6]);
    }

    TEST(1025bit)
    {
        bool ar[1025];
        bit_array b(1025);

        EXPECT_EQ(b.size(), 1025);

        for(unsigned i = 0;i < 1025;i++)
            ar[i] = false;

        for(unsigned i = 0;i < 10000;i++)
        {
            unsigned char action = randomUC();
            size_t ind = randomA<size_t>(1025);

            switch(action & 0x7u)
            {
                case 0:
                    EXPECT_EQ(ar[ind], b.get(ind));
                    break;
                case 1:
                    EXPECT_EQ(ar[ind], b[ind]);
                    break;
                case 2:
                    ar[ind] = false;
                    b.clear(ind);
                    EXPECT_EQ(ar[ind], b[ind]);
                    break;
                case 3:
                    ar[ind] = true;
                    b.set(ind);
                    EXPECT_EQ(ar[ind], b[ind]);
                    break;
                case 4:
                case 5:
                    ar[ind] = (action & 1u);
                    b.set(ind, (action & 1u));
                    EXPECT_EQ(ar[ind], b[ind]);
                    break;
                case 6:
                    if(action == 6)
                    {
                        for(unsigned i = 0;i < 1025;i++)
                            ar[i] = false;
                        b.clear();
                    }
            }
        }

        for(size_t i = 0;i < 1025;i++)
            EXPECT_EQ(ar[i], b[i]);
    }
}