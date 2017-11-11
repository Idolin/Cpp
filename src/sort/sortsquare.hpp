#pragma once

#include "../other/commonmethods.hxx"

template<typename T>
void bubblesort(T *start, T *end)
{
    bool f;
    for(;;)
    {
        f = true;
        for(T *k = start; k < end - 1; k++)
            if(_more(*k, *(k + 1)))
            {
                f = false;
                _swap(k, k + 1);
            }
        if(f)
            return;
        end--;
    }
}

template<typename T>
void shakersort(T *start, T *end)
{
    for(int i = 0;; i++)
    {
        bool f = true;
        for(T *k = start + i; k < end - i - 1; k++)
            if(_more(*k, *(k + 1)))
            {
                f = false;
                _swap(k, k + 1);
            }
        for(T *k = end - i - 2; k > start + i; k--)
            if(_more(*(k - 1), *k))
            {
                f = false;
                _swap(k - 1, k);
            }
        if(f)
            return;
    }
}

template<typename T>
void minmaxsort(T *start, T *end)
{
    T *emin, *emax;
    unsigned cnt = (end - start) / 2;
    for(unsigned i = 0; i < cnt; i++)
    {
        if(_more(*start, *(start + 1)))
            _swap(start, start + 1);
        emin = start;
        emax = emin + 1;
        for(T *k = start + 2; k < end; k++)
            if(_more(*emin, *k))
                emin = k;
            else if(_more(*k, *emax))
                emax = k;
        _swap(start++, emin);
        _swap(--end, emax);
    }
}

template<typename T>
void insertionsort(T *start, T *end)
{
    for(T *t = start + 1; t < end; t++)
        for(T *p = t; p > start; p--)
            if(_more(*(p - 1), *p))
                _swap(p - 1, p);
            else
                break;
}
