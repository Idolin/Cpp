#pragma once

#include <stdio.h>

template<typename T>
inline void _tshow(T x)
{
    puts("Method of displaying not set");
}

template<typename T>
inline bool _more(T x1, T x2)
{
    return x1 > x2;
}

#define _notmore(x, y) (not(_more(x, y)))

template<typename T>
inline T _min(T x1, T x2)
{
    return _more(x1, x2) ? x2 : x1;
}

#ifdef COUNTSWAPS

unsigned long long swapscounter;

#endif // COUNTSWAPS

template<typename T>
inline void _swap(T *x, T *y)
{
    T tmp = *x;
    *x = *y;
    *y = tmp;
#ifdef COUNTSWAPS
    swapscounter++;
#endif // COUNTSWAPS
}

template<typename T>
T to2(T k)
{
    T a = 1;
    while((a < k) and (a > 0))
        a *= 2;
    return a;
}
