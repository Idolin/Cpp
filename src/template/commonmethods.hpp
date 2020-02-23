#pragma once

#include "../debug/def_debug.h"
#include "typemethods.hpp"
#include "type_tags.hpp"

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

template<typename T, typename compare_func<T>::type compare>
inline bool _swap_comp(typename def_get_by<T>::type x1, typename def_get_by<T>::type x2)
{
    return compare(x2, x1);
}

template<typename T, typename compare_func<T>::type compare>
inline bool _not_comp(typename def_get_by<T>::type x1, typename def_get_by<T>::type x2)
{
    return !compare(x1, x2);
}

template<typename T, bool(*compare)(const T&, const T&) = _less<T>>
inline const T& _min(const T& a, const T& b)
{
    return compare(a, b) ? a : b;
}

template<typename T, bool(*compare)(T, T) = _less<T>>
inline T _min(T a, T b)
{
    return compare(a, b) ? a : b;
}

template<typename T, bool(*compare)(const T&, const T&) = _less<T>>
inline const T& _max(const T& a, const T& b)
{
    return compare(a, b) ? b : a;
}

template<typename T, bool(*compare)(T, T) = _less<T>>
inline T _max(T a, T b)
{
    return compare(a, b) ? b : a;
}

namespace
{

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

}

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value ||
                        std::is_pointer<T>::value>::type>
T to2(T k)
{
    if(k < 0)
        return 0;
    T a = 1;
    while((a < k) && (a > 0))
        a <<= 1;
    return a;
}

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value ||
                        std::is_pointer<T>::value>::type> //mask = min(2^x - 1) >= k
constexpr T to_bit_mask(T k)
{
    if(k < 0)
        return std::numeric_limits<T>::min();
    T mask = 0;
    while(mask < k)
        mask = (mask << 1) + 1;
    return mask;
}

template<typename T, typename = typename std::enable_if_t<std::is_integral<T>::value>>
constexpr unsigned char max_bit_pos(T value)
{
    if(value < 0)
        return _typeSeq<T>::bit_length - static_cast<unsigned char>(1);
    T mask = ~static_cast<T>(0);
    unsigned char ans = 255;
    while(mask & value)
        mask <<= 1, ans++;
    return ans;
}

template<typename T,
        typename = typename std::enable_if_t<std::is_integral<T>::value>>
constexpr unsigned char min_bit_pos(T value)
{
    if(value == 0)
        return 255;
    T mask = 1;
    unsigned char ans = 0;
    while(!(mask & value))
        mask <<= 1, ans++;
    return ans;
}

template<typename T,
    typename = typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value>::type>
constexpr inline T sqr(T value)
{
    return value * value;
}

template<typename T, typename PT = unsigned,
    typename = typename std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<PT>::value>>
constexpr T pwr(T value, PT power)
{
    T result = 1;
    T pwr_values[4] = {1, value, sqr(value)};
    while(power)
    {
        if(power & 3)
            result *= pwr_values[power & 1] * pwr_values[power & 2];
        power >>= 2;
        pwr_values[1] = sqr(pwr_values[2]), pwr_values[2] = sqr(pwr_values[1]);
    }
    return result;
}

template<typename T, typename = typename std::enable_if_t<std::is_unsigned<T>::value>>
constexpr bool is_power_of_2(const T value)
{
    if(value & (value - 1))
        return false;
    return (value > 0);
}

template<bool init>
struct once
{
	bool now;
	
	once(): now(init)
	{}
	
	operator bool()
	{
		bool result = now;
		now = ~init;
		return result;
	}
};
