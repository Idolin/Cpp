#pragma once

#include <type_traits>

template<typename T, typename Enable = void>
struct _valueMethods;

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 1)>::type>
{
    constexpr static bool bytesEqual(T x)
    {
        return true;
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 2)>::type>
{
    constexpr static bool bytesEqual(T x)
    {
        return (((x & 0x7f00) >> 8) | ((x & 0x8000) ? 0x80 : 0x00)) == (x & 0xff);
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 4)>::type>
{
    constexpr static bool bytesEqual(T x)
    {
        return ((((x & 0x7fff0000) >> 16) |
                 ((x & 0x80000000) ? 0x8000 : 0x0000)) == (x & 0xffff)) &&
               (((x & 0x0000ff00) >> 8) == (x & 0xff));
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 8)>::type>
{
    constexpr static bool bytesEqual(T x)
    {
        return ((((x & 0x7fffffff00000000) >> 32) |
                 ((x & 0x8000000000000000) ? 0x80000000 :
                  0x00000000)) == (x & 0xffffffff)) &&
                (((x & 0x00000000ffff0000) >> 16) == (x & 0xffff)) &&
                (((x & 0x000000000000ff00) >> 8) == (x & 0xff));
    }
};