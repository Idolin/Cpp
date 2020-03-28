#pragma once

#include <algorithm>
#include <type_traits>

#include "../template/type_tags.hpp"

enum welford_calc
{
    ONLY_MEAN,
    VARIANCE,
    COVARIANCE
};

static inline double merge_mean(double &mean, unsigned long count, double batch_mean, unsigned long batch_count)
{
    double delta = batch_mean - mean;
    unsigned long new_count = count + batch_count;
    if(new_count < 2048 || count > batch_count * 2 || batch_count > count * 2)
        mean += delta * batch_count / (count + 1);
    else
        mean = (batch_count * batch_mean + count * mean) / new_count;
    return delta;
}

struct welford_base
{
protected:
    unsigned long count;
    double mean;
    
    welford_base(): count(0), mean(.0)
    {}
    
public:
    double get_mean() const
    {
        return mean;
    }
    
    unsigned long get_count() const
    {
        return count;
    }
    
protected:
    void update(double value)
    {
        mean += (value - mean) / ++count;
    }
    
    double update_ret_delta(double value)
    {
        double delta = value - mean;
        mean += delta / ++count;
        return delta;
    }
    
    void clear()
    {
        count = 0;
        mean = .0;
    }
};

template<welford_calc mode=ONLY_MEAN>
struct welford
{};

template<>
struct welford<ONLY_MEAN>: welford_base
{
public:
    void update(double value)
    {
        welford_base::update(value);
    }
    
    welford<ONLY_MEAN>& merge(const welford<ONLY_MEAN> &otr)
    {
        merge_mean(mean, count, otr.mean, otr.count);
        count += otr.count;
        return *this;
    }
    
    void clear()
    {
        welford_base::clear();
    }
};

template<>
struct welford<VARIANCE>: welford_base
{
private:
    double m2 = .0;
   
public:
    void update(double value)
    {
        double delta = update_ret_delta(value);
        m2 += delta * (value - mean);
    }
    
    double get_variance() const
    {
        return m2 / count;
    }
    
    double get_sample_variance() const
    {
        return m2 / (count - 1);
    }
    
    welford<VARIANCE>& merge(const welford<VARIANCE> &otr)
    {
        double delta = merge_mean(mean, count, otr.mean, otr.count);
        unsigned long new_count = count + otr.count;
        m2 += otr.m2 + delta * delta * count * otr.count / new_count;
        count = new_count;
        return *this;
    }
    
    void clear()
    {
        welford_base::clear();
        m2 = .0;
    }
};

template<>
struct welford<COVARIANCE>
{
private:
    unsigned long count = 0;
    double mean_x = .0, mean_y = .0;
    double C = .0;
   
public:
    void update(double value_x, double value_y)
    {
        double dx = value_x - mean_x;
        double dy = value_y - mean_y;
        mean_x += dx / ++count;
        mean_y += dy / count;
        C += dx * dy;
    }
    
    unsigned long get_count() const
    {
        return count;
    }
    
    double get_covariance() const
    {
        return C / count;
    }
    
    double get_sample_covariance() const
    {
        return C / (count - 1);
    }
    
    welford<COVARIANCE>& merge(const welford<COVARIANCE> &otr)
    {
        unsigned long new_count = count + otr.count;
        C += otr.C + (mean_x - otr.mean_x) * (mean_y - otr.mean_y) * count * otr.count / new_count;
        merge_mean(mean_x, count, otr.mean_x, otr.count);
        merge_mean(mean_y, count, otr.mean_y, otr.count);
        count = new_count;
        return *this;
    }
    
    void clear()
    {
        count = 0;
        mean_x = mean_y = C = .0;
    }
};
