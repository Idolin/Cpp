#include "../debug/test_def.h"
#include "dsu.hpp"

TEST_PACK(dsu)
{
    TEST_WITH_TYPES(zero_dsu, ((t_adapter::seq<dsu<>, dsu<void, true>, dsu<void, false, true>, dsu<void, true, true>>),
    dsu_type))
    {
        dsu_type d;
        EXPECT_EQ(d.size(), 0);
    }

    TEST_WITH_TYPES(unite, ((t_adapter::seq<dsu<>, dsu<void, true>, dsu<void, false, true>>), dsu_type))
    {
        dsu_type d;
        EXPECT_NE(d.getSetID(0), d.getSetID(2));
        EXPECT_NE(d.getSetID(1), d.getSetID(2));
        d.unite(0, 2);
        EXPECT_EQ(d.getSetID(0), d.getSetID(2));
        EXPECT_NE(d.getSetID(0), d.getSetID(1));
        
        dsu_type d2(2);
        EXPECT_NE(d2.getSetID(0), d2.getSetID(2));
        EXPECT_NE(d2.getSetID(1), d2.getSetID(2));
        d2.unite(0, 2);
        EXPECT_EQ(d2.getSetID(0), d2.getSetID(2));
        EXPECT_NE(d2.getSetID(0), d2.getSetID(1));
        
        
        dsu_type d3(3);
        EXPECT_NE(d3.getSetID(0), d3.getSetID(2));
        EXPECT_NE(d3.getSetID(1), d3.getSetID(2));
        d3.unite(0, 2);
        EXPECT_EQ(d3.getSetID(0), d3.getSetID(2));
        EXPECT_NE(d3.getSetID(0), d3.getSetID(1));
    }

    TEST_WITH_TYPES(unite_range, ((t_adapter::seq<dsu<>, dsu<void, false, true>, dsu<void, true, true>>), dsu_type))
    {
        dsu_type d;
        d.unite(0, 2);
        d.uniteRange(0, 3);
        EXPECT_EQ(d.getSetID(0), d.getSetID(1), "getSetID(0) != getSetID(1)");
        EXPECT_EQ(d.getSetID(1), d.getSetID(2), "getSetID(1) != getSetID(2)");
        EXPECT_NE(d.getSetID(0), d.getSetID(4), "getSetID(0) == getSetID(4)");
        d.unite(3, 4);
        d.uniteRange(2, 4);
        EXPECT_EQ(d.getSetID(0), d.getSetID(4), "getSetID(0) != getSetID(4)");
        
        dsu_type d2(1000);
        d2.unite(0, 2);
        d2.uniteRange(0, 3);
        EXPECT_EQ(d2.getSetID(0), d2.getSetID(1), "getSetID(0) != getSetID(1)");
        EXPECT_EQ(d2.getSetID(1), d2.getSetID(2), "getSetID(1) != getSetID(2)");
        EXPECT_NE(d2.getSetID(0), d2.getSetID(4), "getSetID(0) == getSetID(4)");
        d2.unite(3, 4);
        d2.uniteRange(2, 4);
        EXPECT_EQ(d2.getSetID(0), d2.getSetID(4), "getSetID(0) != getSetID(4)");
    }
}
