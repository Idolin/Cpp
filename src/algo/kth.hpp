#pragma once

#include "../sort/partition.hpp"
#include "../other/rand.h"

template<typename T>
T kth(T *start, T *end, unsigned k)
{
    if(k == 0)
    {
        T min = *start++;
        for(;start < end;start++)
            if(*start < min)
                min = *start;
        return min;
    }
    T mid = *(start + randomA(end - start));
    T *sp = partition(start, end, mid);
    if(k < sp - start)
        return kth(start, sp, k);
    else
        return kth(sp, end, k - (sp - start));
}
