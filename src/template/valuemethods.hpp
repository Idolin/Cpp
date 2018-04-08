#pragma once

template<typename T, typename Enable = void>
struct _valueBytesEquals;

template<typename T>
struct _valueBytesEquals<T, typename std::enable_if<(sizeof(T) == 1)>::type>
{
    constexpr static bool check(T x)
    {
        return true;
    }
};

template<typename T>
struct _valueBytesEquals<T, typename std::enable_if<(sizeof(T) == 2)>::type>
{
    constexpr static bool check(T x)
    {
        return (((x & 0x7fff) >> 8) | ((x < 0) ? 0x80 : 0x00)) == (x & 0xff);
    }
};

template<typename T>
struct _valueBytesEquals<T, typename std::enable_if<(sizeof(T) == 4)>::type>
{
    constexpr static bool check(T x)
    {
        return ((((x & 0x7fffffff) >> 24) | ((x < 0) ? 0x80 : 0x00)) == (x & 0xff)) &&
               (((x & 0x00ffffff) >> 16) == (x & 0xff)) &&
               (((x & 0x0000ffff) >> 8) == (x & 0xff));
    }
};

template<typename T>
struct _valueBytesEquals<T, typename std::enable_if<(sizeof(T) == 8)>::type>
{
    constexpr static bool check(T x)
    {
        return ((((x & 0x7fffffffffffffff) >> 56) |
                 ((x < 0) ? 0x80 : 0x00)) == (x & 0xff)) &&
               (((x & 0x00ffffffffffffff) >> 48) == (x & 0xff)) &&
               (((x & 0x0000ffffffffffff) >> 40) == (x & 0xff)) &&
               (((x & 0x000000ffffffffff) >> 32) == (x & 0xff)) &&
               (((x & 0x00000000ffffffff) >> 24) == (x & 0xff)) &&
               (((x & 0x0000000000ffffff) >> 16) == (x & 0xff)) &&
               (((x & 0x000000000000ffff) >> 8) == (x & 0xff));
    }
};