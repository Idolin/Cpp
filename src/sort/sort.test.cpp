#include "../debug/test_def.h"
#include "sortsquare.hpp"
#include "sortlog.hpp"
#include "sortline.hpp"
#include "../template/arraymethods.hpp"
#include "../other/defdef.h"

TEST_PACK(sort)
{
    TEST(bubblesort)
    {
        SUB_TEST(reverse_order)
        {
            FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, unsigned l, 0, 1, 10, 100, 1000, 10000, {
                unsigned *t = new unsigned[l];
                for(unsigned i = 0;i < l;i++)
                    t[i] = l - i;
                bubblesort(t, t + l);
                EXPECT_TRUE(_checksorted(t, t + l));
            });
        }
    }
}