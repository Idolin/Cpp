#pragma once

#include "../template/commonmethods.hpp"
#include "../template/typemethods.hpp"

template<typename T, typename compare_func<T>::type compare = def_less<T>>
void bubblesort(T *start, T *end)
{
    bool f;
    for(;;)
    {
        f = true;
        for(T *k = start; k < end - 1; k++)
            if(compare(*(k + 1), *k))
            {
                f = false;
                std::swap(*k, *(k + 1));
            }
        if(f)
            return;
        end--;
    }
}

template<typename T, typename compare_func<T>::type compare = def_less<T>>
void shakersort(T *start, T *end)
{
    for(int i = 0;;i++)
    {
        bool f = true;
        for(T *k = start + i;k < end - i - 1;k++)
            if(compare(*(k + 1), *k))
            {
                f = false;
                std::swap(*k, *(k + 1));
            }
        for(T *k = end - i - 2;k > start + i;k--)
            if(compare(*k, *(k - 1)))
            {
                f = false;
                std::swap(*(k - 1), *k);
            }
        if(f)
            return;
    }
}

template<typename T, typename compare_func<T>::type compare = def_less<T>>
void minmaxsort(T *start, T *end)
{
    T *emin, *emax;
    unsigned len_div2 = (end - start) / 2;
    for(unsigned i = 0; i < len_div2; i++)
    {
        if(compare(*(start + 1), *start))
            std::swap(*start, *(start + 1));
        emin = start;
        emax = emin + 1;
        for(T *k = start + 2; k < end; k++)
            if(compare(*k, *emin))
                emin = k;
            else if(compare(*emax, *k))
                emax = k;
        std::swap(*start++, *emin);
        std::swap(*--end, *emax);
    }
}

template<typename T, typename compare_func<T>::type compare = def_less<T>>
void insertionsort(T *start, T *end)
{
    for(T *t = start + 1; t < end; t++)
        for(T *p = t; p > start; p--)
            if(compare(*p, *(p - 1)))
                std::swap(*(p - 1), *p);
            else
                break;
}
