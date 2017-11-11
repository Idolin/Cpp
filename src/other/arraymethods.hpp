#pragma once

#include "commonmethods.hpp"
#include "defdef.h"
#include "../debug/def_debug.h"

#include <cstring>

template<typename T>
inline void _fill(T *start, T *end, int x = '\0')
{
    memset(start, x, (end - start) * sizeof(*start));
}

template<typename T>
inline void _fill(T *start, unsigned len, int x = '\0')
{
    memset(start, x, len * sizeof(*start));
}

template<typename T>
inline void _vfill(T *start, T *end, T x)
{
    if(start < end)
    {
        unsigned dif = end - start;
        while(dif-- > 0)
            *start++ = x;
    }
}

template<typename T>
inline void _vfill(T *start, unsigned len, T x)
{
    while(len-- > 0)
        *start++ = x;
}

inline char *_readstr(unsigned &len)
{
    int c;
    char *str = new char[len];
    unsigned i = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        str[i++] = (char) c;
    len = i;
    return str;
}

template<typename T = unsigned>
inline void _read(T *start, T *end, const char *scf = "%u")
{
    for(T *i = start; i < end; i++)
        scanf(scf, i);
}

template<typename T = unsigned>
inline void _read(T *start, unsigned len, const char *scf = "%u")
{
    for(unsigned i = 0; i < len; i++)
        scanf(scf, start++);
}

template<typename T = unsigned>
inline T *_read(unsigned len, const char *scf = "%u")
{
    T *r = new T[len];
    for(unsigned i = 0; i < len; i++)
        scanf(scf, r + i);
    return r;
}

template<typename T>
inline void _copy(T *const __restrict__ start, T *end, const T *const __restrict__ source)
{
    memcpy(start, source, (end - start) * sizeof(*start));
}

template<typename T>
inline void _copy(T *const __restrict__ start, unsigned len, const T *const __restrict__ source)
{
    memcpy(start, source, len * sizeof(*start));
}

template<typename T>
inline void _copy_a(T *const start, T *end, T *const source)
{
    memccpy(start, source, (end - start) * sizeof(*start));
}

template<typename T>
inline void _copy_a(T *const start, unsigned len, T *const source)
{
    memccpy(start, source, len * sizeof(*start));
}

template<typename T>
inline void _display(T *start, T *end, const char *prf = "%u", const char *del = " ")
{
    ASSERT(start <= end);
    if(start == end)
        return;
    printf(prf, *start);
    while(++start < end)
    {
        fputs(del, stdout);
        printf(prf, *start);
    }
    putchar('\n');
}

template<typename T>
inline void _display(T *start, unsigned len, const char *prf = "%u", const char *del = " ")
{
    if(len == 0)
        return;
    printf(prf, *start);
    while(len-- > 1)
    {
        fputs(del, stdout);
        printf(prf, *++start);
    }
    putchar('\n');
}

template<typename T>
inline void _tdisplay(T *start, T *end, const char *del = " ")
{
    ASSERT(start <= end);
    if(start == end)
        return;
    _tshow(*start);
    while(++start < end)
    {
        fputs(del, stdout);
        _tshow(*start);
    }
    putchar('\n');
}

template<typename T>
inline void _tdisplay(T *start, unsigned len, const char *del = " ")
{
    if(len == 0)
        return;
    _tshow(*start);
    while(len-- > 1)
    {
        fputs(del, stdout);
        _tshow(*++start);
    }
    putchar('\n');
}

template<typename T>
inline void _display2D(T **array, unsigned length, unsigned height, const char *prf = "%u", const char *del = " ",
                       const char *ldel = "\n")
{
    if((height == 0) or (length == 0))
        return;
    for(unsigned y = 0; y < height; y++)
    {
        printf(prf, array[y]);
        for(unsigned x = 1; x < length; x++)
        {
            fputs(del, stdout);
            printf(prf, array[y] + x);
        }
        printf(ldel, stdout);
    }
}

template<typename T>
inline T _min(T *start, T *end)
{
    T emin = *start;
    while(++start < end)
        if(_more(emin, *start))
            emin = *start;
    return emin;
}

template<typename T>
inline T _min(T *start, unsigned len)
{
    T emin = *start;
    while(len-- > 1)
        if(_more(emin, *++start))
            emin = *start;
    return emin;
}

template<typename T>
inline T _max(T *start, T *end)
{
    T emax = *start;
    while(++start < end)
        if(_more(*start, emax))
            emax = *start;
    return emax;
}

template<typename T>
inline T _max(T *start, unsigned len)
{
    T emax = *start;
    while(len-- > 1)
        if(_more(*++start, emax))
            emax = *start;
    return emax;
}

template<typename T>
inline unsigned _minInd(T *start, T *end)
{
    T vmin = *start;
    unsigned ind = 0;
    unsigned len = end - start;
    while(len > 0)
        if(_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
inline unsigned _minInd(T *start, unsigned len)
{
    T vmin = *start;
    unsigned ind = 0;
    while(len > 0)
        if(_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
inline unsigned _maxInd(T *start, T *end)
{
    T vmax = *start;
    unsigned ind = 0;
    unsigned len = end - start;
    while(len > 0)
        if(_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename T>
inline unsigned _maxInd(T *start, unsigned len)
{
    T vmax = *start;
    unsigned ind = 0;
    while(len > 0)
        if(_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename Ta, typename Ts>
inline Ts _sum(Ta *start, unsigned len)
{
    Ts sum = 0;
    while(len-- > 0)
        sum += start[len];
    return sum;
}

template<typename Ta, typename Ts>
inline Ts _sum(Ta *start, Ta *end)
{
    Ts sum = 0;
    unsigned len = end - start;
    while(len-- > 0)
        sum += start[len];
    return sum;
}

template<typename T>
inline bool _checksorted(T *start, T *end)
{
    while(++start < end)
        if(_more(*(start - 1), *start))
            return false;
    return true;
}

template<typename T>
inline bool _checksorted(T *start, unsigned len)
{
    if(len == 0)
        return true;
    while(--len > 0)
        if(_more(*start++, *(start + 1)))
            return false;
    return true;
}

template<typename T>
inline T **_newArray2d(unsigned height, unsigned lenght)
{
    T **array = new T *[height];
    while(height > 0)
        array[--height] = new T[lenght];
    return array;
}

template<typename T>
inline void _deleteArray2d(T **array, unsigned height)
{
    while(height > 0)
        delete[] array[--height];
    delete[] array;
}

#ifdef INDEXARRAY

unsigned *indexArray;

#endif // INDEXARRAY

template<typename T1, typename T2>
inline void _adswap(T1 *array1, T2 *array2, unsigned f, unsigned s)
{
    T1 tmp1 = array1[f];
    array1[f] = array1[s];
    array1[s] = tmp1;
    T2 tmp2 = array2[f];
    array2[f] = array2[s];
    array2[s] = tmp2;
#ifdef COUNTSWAPS
    swapscounter+=2;
#endif // COUNTSWAPS
#ifdef INDEXARRAY
    unsigned tmpInd = indexArray[f];
    indexArray[f] = indexArray[s];
    indexArray[s] = tmpInd;
#endif // INDEXARRAY
}

template<typename T>
inline void _aswap(T *array, unsigned f, unsigned s)
{
    T tmp = array[f];
    array[f] = array[s];
    array[s] = tmp;
#ifdef COUNTSWAPS
    swapscounter++;
#endif // COUNTSWAPS
#ifdef INDEXARRAY
    unsigned tmpInd = indexArray[f];
    indexArray[f] = indexArray[s];
    indexArray[s] = tmpInd;
#endif // INDEXARRAY
}

inline bool equals(const char *const a, const char *const b)
{
    for(unsigned i = 0;; i++)
        if(a[i] != b[i])
            return false;
        elif(a[i] == '\0')
            return true;
}
