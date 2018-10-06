#pragma once

#include "../template/commonmethods.hpp"
#include "../template/typemethods.hpp"
#include <algorithm>
#include <functional>

template<typename T, typename compare_func<T>::type compare = _less<T>>
T *partition(T *start, T *end, T df)
{
    for(;start < end;start++)
        if(!compare(*start, df))
        {
            while(compare(df, *--end))
                if(start == end)
                    return start;
            std::swap(*start, *end);
        }
    return end;
}
