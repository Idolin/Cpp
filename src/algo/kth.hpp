#pragma once

#include "../sort/partition.hpp"

template<typename T>
T kth(T *start, T *end, unsigned k, unsigned count = 0, T min = (T) 0)
{
    if(k == 0)
    {
        T min = *start;
        for(T *sp = start + 1; sp <= end; sp++)
            if(*sp < min)
                min = *sp;
        return min;
    }
    T *sp = start + randm(end - start + 1), *ep = start + randm(end - start + 1);
    T mid = *(start + (end - start) / 2);
    if(*sp > *ep)
    {
        if(mid > *sp)
            mid = *sp;
        else if(mid < *ep)
            mid = *ep;
    } else if(mid > *ep)
        mid = *ep;
    else if(mid < *sp)
        mid = *sp;
    ep = start + count;
    if(k < ep - start)
        return min;
    sp = partition(start, end, mid, count, min);
    if(k < sp - start)
        return kth(ep, sp - 1, k - (ep - start));
    else
        return kth(sp, end, k - (sp - start), count, mid);
}
