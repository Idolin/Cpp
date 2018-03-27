#pragma once

#include <stdio.h>
#include "defdef.h"

template<typename T>
struct _format
{};

template<>
struct _format<unsigned char>
{
    constexpr static const char *const specifier = "%u";
};

template<>
struct _format<char>
{
    constexpr static const char *const specifier = "%c";
};

template<>
struct _format<unsigned short>
{
    constexpr static const char *const specifier = "%u";
};

template<>
struct _format<short>
{
    constexpr static const char *const specifier = "%d";
};

template<>
struct _format<unsigned>
{
    constexpr static const char *const specifier = "%u";
};

template<>
struct _format<int>
{
    constexpr static const char *const specifier = "%d";
};

template<>
struct _format<unsigned long>
{
    constexpr static const char *const specifier = "%lu";
};

template<>
struct _format<long>
{
    constexpr static const char *const specifier = "%ld";
};

template<>
struct _format<unsigned long long>
{
    constexpr static const char *const specifier = "%llu";
};

template<>
struct _format<long long>
{
    constexpr static const char *const specifier = "%lld";
};

template<typename T>
inline void _display(const T *const start, const T *const end, const char *const prf = _format<T>::specifier,
                     const char *const del = " ")
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
inline void _display(T *start, unsigned len, const char *prf = _format<T>::specifier, const char *del = " ")
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
inline void
_display2D(T **array, unsigned length, unsigned height, const char *prf = _format<T>::specifier, const char *del = " ",
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