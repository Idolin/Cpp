#pragma once

#include "../other/commonmethods.hpp"
#include <functional>

template<typename T, bool (*compare)(const T &, const T &) = _less<T>>
T *partition_3(T *start, const T *end, T &df, unsigned &count, T &min)
{
    T *sp = start;
    if(count > 0)
    {
        if(not(compare(df, min) or compare(min, df)))
            df++;
        sp += count;
        count = 0;
        T *ep = sp;
        while(ep < end)
            if(not compare(df, *ep))
            {
                if(not(compare(*ep, min) or compare(min, *ep)))
                {
                    while(not(compare(*start, min) or compare(min, *start)))
                        start++;
                    _swap(start++, ep);
                } else
                {
                    if(compare(*ep, df))
                        _swap(sp++, ep);
                    else
                        count++;
                    ep++;
                }
            } else
                ep++;
    } else
    {
        for(T *ep = sp; ep < end; ep++)
            if(not compare(df, *ep))
            {
                if(compare(*ep, df))
                    _swap(sp++, ep);
                else
                    count++;
            }
    }
    return sp;
}

template<typename T, bool (*compare)(const T &, const T &) = _less<T>>
T *partition(T *start, T *end, T df)
{
    for(T *ep = end; start < ep; start++)
        if(not compare(*start, df))
        {
            while(compare(df, *--ep))
                if(start == ep)
                    return start;
            std::swap(start, ep);
        }
    return start;
}
