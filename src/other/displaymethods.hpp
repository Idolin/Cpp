#pragma once

#include <stdio.h>
#include "defdef.h"

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
inline typename std::enable_if<std::is_same<std::remove_reference<T>, const size_t>::value &&
                               !std::is_integral<size_t>::value>::type _tshow(T& x)
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
    printf(":%u", x);
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

template<typename T, void (*show)(const T&) = &_tshow>
inline void _tdisplay(T *a, unsigned long len, const char *del = ", ")
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

template<typename T, void (*show)(const T&) = &_tshow>
inline void _tdisplay(T *a, unsigned long start, unsigned long end, const char* del = ", ")
{
    _tdisplay<T, show>(a + start, end - start, del);
}