#include "../debug/test_def.h"
#include "dsu.hpp"

TEST_PACK(dsu)
{
    TEST(zero_dsu)
    {
        FOR_CLASS(d, dsu<>(), (dsu<void, true>()), (dsu<void, false, true>()),
                  (dsu<void, true, true>()), {
            EXPECT_EQ(d.size(), 0);
        });
    }

    TEST(unite)
    {
        FOR_CLASS(d, dsu<>(), dsu<>(3), (dsu<void, true>(2)), (dsu<void, false, true>()), {
            EXPECT_NE(d.getSetID(0), d.getSetID(2));
            EXPECT_NE(d.getSetID(1), d.getSetID(2));
            d.unite(0, 2);
            EXPECT_EQ(d.getSetID(0), d.getSetID(2));
            EXPECT_NE(d.getSetID(0), d.getSetID(1));
        });
    }

    TEST(unite_range)
    {
        FOR_CLASS(d, dsu<>(), dsu<>(1000), (dsu<void, false, true>()), (dsu<void, true, true>()), {
            d.unite(0, 2);
            d.uniteRange(0, 3);
            EXPECT_EQ(d.getSetID(0), d.getSetID(1));
            EXPECT_EQ(d.getSetID(1), d.getSetID(2));
            EXPECT_NE(d.getSetID(0), d.getSetID(4));
            d.unite(3, 4);
            d.uniteRange(2, 4);
            EXPECT_EQ(d.getSetID(0), d.getSetID(4));
        });
    }
}