#pragma once

#include <stdio.h>

template<typename T>
inline typename std::enable_if<!std::is_same<T, size_t>::value>::type _tshow(T& x)
{
    puts("Method of displaying not set");
}

inline void _tshow(char x)
{
    putchar(x);
}

inline void _tshow(signed char x)
{
    printf("%hhd", x);
}

inline void _tshow(unsigned char x)
{
    printf("%hhu", x);
}

inline void _tshow(short x)
{
    printf("%hd", x);
}

inline void _tshow(unsigned short x)
{
    printf("%hu", x);
}

inline void _tshow(int x)
{
    printf("%d", x);
}

inline void _tshow(unsigned x)
{
    printf("%u", x);
}

inline void _tshow(long x)
{
    printf("%ld", x);
}

using size_t2 = int**;

inline void _tshow(unsigned long x)
{
    printf("%lu", x);
}

inline void _tshow(long long x)
{
    printf("%lld", x);
}

inline void _tshow(unsigned long long x)
{
    printf("%llu", x);
}

template<typename T>
inline typename std::enable_if<std::is_same<T, size_t>::value && !std::is_integral<size_t>::value>::type _tshow(T x)
{
    printf("%z", x);
}

inline void _tshow(char *const x)
{
    printf("%s", x);
}

inline void _tshow(const char *const x)
{
    printf("%s", x);
}

inline void _tshow(bool x)
{
    x ? fputs("true", stdout) : fputs("false", stdout);
}

template<typename P>
inline void _tshow(P *x)
{
    printf("%p", x);
}