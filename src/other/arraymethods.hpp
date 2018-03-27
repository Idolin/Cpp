#pragma once

#include "commonmethods.hpp"
#include "defdef.h"
#include "../debug/def_debug.h"

#include <algorithm>
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

template<typename T = unsigned, typename SizeType=unsigned>
inline void _read(T *start, SizeType len, const char *scf = "%u")
{
    for(unsigned i = 0; i < len; i++)
        scanf(scf, start++);
}

template<typename T = unsigned, typename SizeType=unsigned>
inline T *_read(SizeType len, const char *scf = "%u")
{
    T *r = new T[len];
    for(unsigned i = 0; i < len; i++)
        scanf(scf, r + i);
    return r;
}

template<typename T>
inline typename std::enable_if<std::is_pod<T>::value>::type
    _copy(T *__restrict__ start, T *end, const T *__restrict__ source)
{
    memcpy(start, source, (end - start) * sizeof(*start));
}

template<typename T>
inline typename std::enable_if<!std::is_pod<T>::value>::type
    _copy(T *__restrict__ start, T *end, const T *__restrict__ source)
{
    while(start < end)
        *start++ = *source++;
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<std::is_pod<T>::value>::type
    _copy(T *__restrict__ start, SizeType len, const T *__restrict__ source)
{
    memcpy(start, source, len * sizeof(*start));
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<!std::is_pod<T>::value>::type
    _copy(T *__restrict__ start, SizeType len, const T *__restrict__ source)
{
    while(len--)
        *start++ = *source++;
}

template<typename T>
inline typename std::enable_if<std::is_pod<T>::value>::type
    _copy_a(T *start, T *end, const T *source)
{
    memmove(start, source, (end - start) * sizeof(*start));
}

template<typename T>
inline typename std::enable_if<!std::is_pod<T>::value>::type
    _copy_a(T *start, T *end, const T *source)
{
    if(start < source)
        while(start < end)
            *start++ = *source++;
    else
    {
        source += end - start;
        while(start < end)
            *--end = *--source;
    }
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<std::is_pod<T>::value>::type
    _copy_a(T *start, SizeType len, const T *source)
{
    memmove(start, source, len * sizeof(*start));
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<!std::is_pod<T>::value>::type
    _copy_a(T *start, SizeType len, const T *source)
{
    if(start < source)
        while(len--)
            *start++ = *source++;
    else
    {
        source += len;
        start += len;
        while(len--)
            *--start = *--source;
    }
}

template<typename T>
inline typename std::enable_if<std::is_pod<T>::value>::type
    _move(T *__restrict__ start, T *end, T *__restrict__ source)
{
    memcpy(start, source, (end - start) * sizeof(*start));
}

template<typename T>
inline typename std::enable_if<!std::is_pod<T>::value>::type
    _move(T *__restrict__ start, T *end, T *__restrict__ source)
{
    while(start < end)
        *start++ = std::move(*source++);
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<std::is_pod<T>::value>::type
    _move(T *__restrict__ start, SizeType len, T *__restrict__ source)
{
    memcpy(start, source, len * sizeof(*start));
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<!std::is_pod<T>::value>::type
    _move(T *__restrict__ start, SizeType len, T *__restrict__ source)
{
    while(len--)
        *start++ = std::move(*source++);
}

template<typename T, typename SizeType=unsigned>
inline T* _resize(T *start, T *end, SizeType new_length)
{
    DEBUGLVLIFMSG(3, new_length < (end - start), "new size lesser than old, some elements will be deleted!");
    T *new_array = new T[new_length];
    _move(new_array, std::min(end - start, new_length), start);
    delete [] start;
    return new_array;
}

template<typename T, typename SizeType=unsigned>
inline T* _resize(T *start, SizeType now_length, SizeType new_length)
{
    DEBUGLVLIFMSG(3, new_length < now_length, "new size lesser than old, some elements will be deleted!");
    T *new_array = new T[new_length];
    _move(new_array, std::min(now_length, new_length), start);
    delete [] start;
    return new_array;
}

template<typename T, typename SizeType=unsigned>
inline void _mult_array(T *const start, SizeType len, unsigned times)
{
    unsigned done = 1;
    unsigned times_shift = times >> 1;
    SizeType clen = len;
    while(done <= times_shift)
    {
        _copy_a(start + clen, clen, start);
        clen <<= 1;
        done <<= 1;
    }
    if(times -= done)
        _copy_a(start + clen, len * times, start);
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

template<typename T, bool (*compare)(const T &, const T &) = _less<T>>
inline bool _checksorted(T *start, T *end)
{
    while(++start < end)
        if(compare(*start, *(start - 1)))
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
