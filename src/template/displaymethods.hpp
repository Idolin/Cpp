#pragma once

#include <stdio.h>
#include "../other/defdef.h"
#include "../debug/def_debug.h"
#include "typemethods.hpp"

template<typename T>
inline void _display(const T *start, const T *end, const char *prf = _typeSeq<T>::specifier,
                     const char *del = " ", const char *end_s = "\n")
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
    printf("%s", end_s);
}

template<typename T, typename SizeType = unsigned,
        typename = typename std::enable_if<std::is_integral<SizeType>::value>::type>
inline void _display(const T *start, SizeType len, const char *prf = _typeSeq<T>::specifier, const char *del = " ",
                     const char *end_s = "\n")
{
    if(len == 0)
        return;
    printf(prf, *start);
    while(len-- > 1)
    {
        fputs(del, stdout);
        printf(prf, *++start);
    }
    printf("%s", end_s);
}

template<typename T, typename SizeTypeL = unsigned, typename SizeTypeH = unsigned>
inline void _display2D(const T *const *array, SizeTypeL length, SizeTypeH height,
                       const char *prf = _typeSeq<T>::specifier, const char *del = " ",
                       const char *ldel = "\n")
{
    if((height == 0) or (length == 0))
        return;
    for(unsigned y = 0; y < height; y++)
    {
        printf(prf, array[y][0]);
        for(unsigned x = 1; x < length; x++)
        {
            fputs(del, stdout);
            printf(prf, array[y][x]);
        }
        printf(ldel, stdout);
    }
}

template<typename T>
inline void _tshow(const T& x)
{
    if(std::is_pointer<T>::value)
    {
        if(std::is_same<typename std::remove_cv<typename std::remove_pointer<
                typename std::remove_reference<T>::type>::type>::type, char>::value)
            printf("%s", x);
        else
            printf("%p", x);
    }
    else
        puts("Method of displaying not set");
}

template<typename T>
inline typename std::enable_if<std::is_same<typename clear_type<T>::type, size_t>::value &&
                               !std::is_integral<size_t>::value>::type _tshow(const T &x)
{
    printf("%z", x);
}

template<>
inline void _tshow(const char& x)
{
    putchar(x);
}

template<>
inline void _tshow(const signed char& x)
{
    printf("%hhd", x);
}

template<>
inline void _tshow(const unsigned char& x)
{
    printf("%hhu", x);
}

template<>
inline void _tshow(const short& x)
{
    printf("%hd", x);
}

template<>
inline void _tshow(const unsigned short& x)
{
    printf("%hu", x);
}

template<>
inline void _tshow(const int& x)
{
    printf("%d", x);
}

template<>
inline void _tshow(const unsigned& x)
{
    printf("%u", x);
}

template<>
inline void _tshow(const long& x)
{
    printf("%ld", x);
}

template<>
inline void _tshow(const unsigned long& x)
{
    printf("%lu", x);
}

template<>
inline void _tshow(const long long& x)
{
    printf("%lld", x);
}

template<>
inline void _tshow(const unsigned long long& x)
{
    printf("%llu", x);
}

template<>
inline void _tshow(const bool& x)
{
    x ? fputs("true", stdout) : fputs("false", stdout);
}

template<typename T, void (*show)(const T&) = _tshow<T>>
inline void _tdisplay(const T *a, unsigned long len, const char *del = ", ")
{
    if(len--)
    {
        show(a[0]);
        while(len--)
        {
            fputs(del, stdout);
            show(*++a);
        }
    }
    putchar('\n');
}

template<typename T, void (*show)(const T&) = _tshow<T>>
inline void _tdisplay(const T *a, unsigned long start, unsigned long end, const char *del = ", ")
{
    _tdisplay<T, show>(a + start, end - start, del);
}
