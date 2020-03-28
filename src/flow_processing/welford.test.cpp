#include "../debug/test_def.h"

#include "welford.hpp"

TEST_PACK(welford)
{
    TEST(mean_from_0_to_1)
    {
        welford<ONLY_MEAN> w;
        
        unsigned long to = 10000000;
        for(unsigned long i = 0;i <= to;i++)
            w.update(static_cast<double>(i) / to);
        EXPECT_DOUBLE_EQ(w.get_mean(), .5);
        EXPECT_EQ(w.get_count(), to + 1);
        
        w.clear();
        EXPECT_EQ(w.get_count(), 0);
        w.update(33);
        EXPECT_EQ(w.get_count(), 1);
        EXPECT_EQ(w.get_mean(), 33);
    }
    
    TEST(mean_from_10000_to_20000)
    {
        welford<ONLY_MEAN> w;
        
        unsigned long to = 10000000;
        for(unsigned long i = 0;i <= to;i++)
            w.update(10000 + static_cast<double>(i * 10000) / to);
        EXPECT_NEAR(w.get_mean(), 15000, 0.00001);
    }
    
    TEST(mean_merge_0_and_1)
    {
        welford<ONLY_MEAN> w1, w2;
        
        unsigned long to = 1000000;
        for(unsigned long i = 0;i <= to;i++)
        {
            w1.update(0);
            w2.update(1);
        }
        
        EXPECT_DOUBLE_EQ(w1.get_mean(), 0);
        EXPECT_DOUBLE_EQ(w2.get_mean(), 1);
        EXPECT_EQ(w1.get_count(), to * 1 + 1);
        EXPECT_EQ(w2.get_count(), to * 1 + 1);
        EXPECT_DOUBLE_EQ(w1.merge(w2).get_mean(), 0.5);
        EXPECT_EQ(w1.get_count(), to * 2 + 2);
    }
    
    TEST(mean_merge_1000_and_4000)
    {
        welford<ONLY_MEAN> w1, w2;
        
        unsigned long to = 1000000;
        for(unsigned long i = 0;i < to;i++)
        {
            w1.update(1000);
            w1.update(1000);
            w2.update(4000);
        }
        
        EXPECT_DOUBLE_EQ(w1.merge(w2).get_mean(), 2000);
    }
    
    TEST(variance_0_and_1)
    {
        welford<VARIANCE> w;
        
        unsigned long to = 1000000;
        for(unsigned long i = 0;i < to;i++)
        {
            w.update(0);
            w.update(1);
        }
        
        EXPECT_DOUBLE_EQ(w.get_mean(), 0.5);
        EXPECT_DOUBLE_EQ(w.get_variance(), 0.25); 
    }
}
