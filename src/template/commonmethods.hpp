#pragma once

#include "typemethods.hpp"
#include "struct_tags.hpp"

#include <algorithm>
#include <type_traits>

template<typename T, typename = typename def_get_by_value<T>::type>
inline bool _less(T x1, T x2)
{
    return x1 < x2;
}

template<typename T, typename = typename std::enable_if<
        is_less_comparable<T>::value, typename def_get_by_reference<T>::type>::type>
inline bool _less(const T& x1, const T& x2)
{
    return x1 < x2;
}

template<typename T, typename = typename def_get_by_value<T>::type>
inline bool _more(T x1, T x2)
{
    return _less(x2, x1);
}

template<typename T, typename = typename def_get_by_reference<T>::type>
inline bool _more(const T& x1, const T& x2)
{
    return _less(x2, x1);
}

#define _notmore(x, y) (not(_more(x, y)))

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

template<typename T, bool(*compare)(const T&, const T&) = _less<T>>
inline T& _min(const T& a, const T& b)
{
    return compare(a, b) ? a : b;
}

template<typename T, bool(*compare)(T, T) = _less<T>>
inline T _min(T a, T b)
{
    return compare(a, b) ? a : b;
}

template<typename T, bool(*compare)(const T&, const T&) = _less<T>>
inline T& _max(const T& a, const T& b)
{
    return compare(a, b) ? b : a;
}

template<typename T, bool(*compare)(T, T) = _less<T>>
inline T _max(T a, T b)
{
    return compare(a, b) ? b : a;
}

template<typename T, typename T2, typename Enable = void>
struct _getMMinType;

template<typename T, typename T2>
struct _getMMinType<T, T2, typename std::enable_if<
        std::is_signed<T>::value && std::is_signed<T2>::value ||
        std::is_unsigned<T>::value && std::is_unsigned<T2>::value>::type>
{
    typedef typename _getMinType<T, T2>::type type;
};

template<typename T, typename T2>
struct _getMMinType<T, T2, typename std::enable_if<
        std::is_signed<T>::value && std::is_unsigned<T2>::value>::type>
{
    typedef T type;
};

template<typename T, typename T2>
struct _getMMinType<T, T2, typename std::enable_if<
        std::is_unsigned<T>::value && std::is_signed<T2>::value>::type>
{
    typedef T2 type;
};

template<typename T, typename T2, typename Enable = void>
struct _getMMaxType;

template<typename T, typename T2>
struct _getMMaxType<T, T2, typename std::enable_if<
        std::is_signed<T>::value && std::is_signed<T2>::value ||
        std::is_unsigned<T>::value && std::is_unsigned<T2>::value>::type>
{
    typedef typename _getMaxType<T, T2>::type type;
};

template<typename T, typename T2>
struct _getMMaxType<T, T2, typename std::enable_if<
        std::is_signed<T>::value && std::is_unsigned<T2>::value>::type>
{
    typedef typename std::make_unsigned<
            typename _getMaxType<T, T2>::type>::type type;
};

template<typename T, typename T2>
struct _getMMaxType<T, T2, typename std::enable_if<
        std::is_unsigned<T>::value && std::is_signed<T2>::value>::type>
{
    typedef typename _getMMaxType<T2, T>::type type;
};

template<typename T, typename T2, typename =
    typename std::enable_if<!std::is_same<T, T2>::value>::type>
inline typename _getMMinType<T, T2>::type _min(T a, T2 b)
{
    using maxType = typename _getMMaxType<T, T2>::type;
    using minType = typename _getMMinType<T, T2>::type;
    if(a < 0 || b < 0)
    {
        if(a >= 0)
            return static_cast<minType>(b);
        if(b >= 0)
            return static_cast<minType>(a);
        return static_cast<minType>(a) < static_cast<minType>(b) ?
               static_cast<minType>(a) : static_cast<minType>(b);
    }
    return static_cast<maxType>(a) < static_cast<maxType>(b) ?
           static_cast<minType>(a) : static_cast<minType>(b);
}

template<typename T, typename T2, typename =
    typename std::enable_if<!std::is_same<T, T2>::value>::type>
inline typename _getMMaxType<T, T2>::type _max(T a, T2 b)
{
    using maxType = typename _getMMaxType<T, T2>::type;
    using minType = typename _getMMinType<T, T2>::type;
    if(a < 0 || b < 0)
    {
        if(a >= 0)
            return static_cast<maxType>(a);
        if(b >= 0)
            return static_cast<maxType>(b);
        return static_cast<minType>(a) < static_cast<minType>(b) ?
               static_cast<maxType>(b) : static_cast<maxType>(a);
    }
    return static_cast<maxType>(a) < static_cast<maxType>(b) ?
           static_cast<maxType>(b) : static_cast<maxType>(a);
}

template<typename T>
T to2(T k)
{
    T a = 1;
    while((a < k) and (a > 0))
        a <<= 1;
    return a;
}

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
constexpr unsigned char max_bit_pos(T value)
{
    if(value < 0)
        return sizeof(T) << 3;
    T mask = ~0;
    unsigned char ans = 0;
    while(mask & value)
    {
        mask <<= 1;
        ans++;
    }
    return ans;
}

template<typename T,
        typename = typename std::enable_if<std::is_integral<T>::value>::type>
constexpr unsigned char min_bit_pos(T value)
{
    if(value == std::numeric_limits<T>::min())
        return (value < 0) ? 255 : (sizeof(T) << 3);
    for(unsigned char i = 0;; i++)
        if(value & (1 << i))
            return i;
}