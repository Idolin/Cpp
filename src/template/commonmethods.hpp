#pragma once

#include "../debug/def_debug.h"
#include "typemethods.hpp"
#include "type_tags.hpp"

#include <algorithm>
#include <limits>
#include <type_traits>

template<typename T,
         typename = std::enable_if_t<std::is_scalar<T>::value>>
inline bool def_less(T x1, T x2)
{
    return x1 < x2;
}

template<typename T,
         typename = std::enable_if_t<!std::is_scalar<T>::value && is_less_comparable_v<T>>>
inline bool def_less(const T& x1, const T& x2)
{
    return x1 < x2;
}

template<typename T,
         typename = std::enable_if_t<std::is_scalar<T>::value>>
inline bool def_more(T x1, T x2)
{
    return def_less(x2, x1);
}

template<typename T,
         typename = std::enable_if_t<!std::is_scalar<T>::value && is_less_comparable_v<T>>>
inline bool def_more(const T& x1, const T& x2)
{
    return def_less(x2, x1);
}

template<typename T, typename compare_func<T>::type compare>
inline bool def_swap_comp(typename def_get_by<T>::type x1, typename def_get_by<T>::type x2)
{
    return compare(x2, x1);
}

template<typename T, typename compare_func<T>::type compare>
inline bool def_not_comp(typename def_get_by<T>::type x1, typename def_get_by<T>::type x2)
{
    return !compare(x1, x2);
}

namespace
{

    template<typename T, typename T2, typename Enable = void>
    struct _GetMinReturnType;

    template<typename T, typename T2>
    struct _GetMinReturnType<T, T2,
            std::enable_if_t<(std::is_signed<T>::value && std::is_signed<T2>::value) ||
            (std::is_unsigned<T>::value && std::is_unsigned<T2>::value)>>
    {
        typedef typename get_smaller_type<T, T2>::type type;
    };

    template<typename T, typename T2>
    struct _GetMinReturnType<T, T2,
            std::enable_if_t<std::is_signed<T>::value && std::is_unsigned<T2>::value>>
    {
        typedef T type;
    };

    template<typename T, typename T2>
    struct _GetMinReturnType<T, T2,
            std::enable_if_t<std::is_unsigned<T>::value && std::is_signed<T2>::value>>
    {
        typedef T2 type;
    };

    template<typename T, typename T2, typename Enable = void>
    struct _GetMaxReturnType;

    template<typename T, typename T2>
    struct _GetMaxReturnType<T, T2,
            std::enable_if_t<(std::is_signed<T>::value && std::is_signed<T2>::value) ||
                (std::is_unsigned<T>::value && std::is_unsigned<T2>::value)>>
    {
        typedef typename get_bigger_type<T, T2>::type type;
    };

    template<typename T, typename T2>
    struct _GetMaxReturnType<T, T2,
            std::enable_if_t<std::is_signed<T>::value && std::is_unsigned<T2>::value>>
    {
        typedef typename std::make_unsigned<
                typename get_bigger_type<T, T2>::type>::type type;
    };

    template<typename T, typename T2>
    struct _GetMaxReturnType<T, T2,
            std::enable_if_t<std::is_unsigned<T>::value && std::is_signed<T2>::value>>
    {
        typedef typename _GetMaxReturnType<T2, T>::type type;
    };

}

template<typename T, compare_func_t<T> compare = def_less<T>, typename T2 = T,
         typename = std::enable_if_t<std::is_same<T, T2>::value && std::is_scalar<T>::value>>
constexpr T min(T a, T2 b)
{
    return compare(a, b) ? a : b;
}

template<typename T, compare_func_t<T> compare = def_less<T>, typename T2 = T,
         typename = std::enable_if_t<std::is_same<T, T2>::value && !std::is_scalar<T>::value>>
constexpr T& min(const T& a, const T2& b)
{
    return compare(a, b) ? a : b;
}

template<typename T, typename T2,
         typename = std::enable_if_t<!std::is_same<T, T2>::value>>
constexpr typename _GetMinReturnType<T, T2>::type min(T a, T2 b)
{
    using maxType = typename _GetMaxReturnType<T, T2>::type;
    using minType = typename _GetMinReturnType<T, T2>::type;
    if(a < 0 || b < 0)
    {
        if(a >= 0)
            return static_cast<minType>(b);
        if(b >= 0)
            return static_cast<minType>(a);
        return (static_cast<minType>(a) < static_cast<minType>(b)) ?
               static_cast<minType>(a) : static_cast<minType>(b);
    }
    return static_cast<maxType>(a) < static_cast<maxType>(b) ?
           static_cast<minType>(a) : static_cast<minType>(b);
}

template<typename T, compare_func_t<T> compare = def_less<T>, typename T2 = T, typename = std::enable_if_t<std::is_same<T, T2>::value && std::is_scalar<T>::value>>
constexpr T max(T a, T2 b)
{
    return compare(a, b) ? b : a;
}

template<typename T, compare_func_t<T> compare = def_less<T>, typename T2 = T, typename = std::enable_if_t<std::is_same<T, T2>::value && !std::is_scalar<T>::value>>
constexpr T& max(const T& a, const T2& b)
{
    return compare(a, b) ? b : a;
}

template<typename T, typename T2,
         typename = std::enable_if_t<!std::is_same<T, T2>::value>>
constexpr typename _GetMaxReturnType<T, T2>::type max(T a, T2 b)
{
    using maxType = typename _GetMaxReturnType<T, T2>::type;
    using minType = typename _GetMinReturnType<T, T2>::type;
    if(a < 0 || b < 0)
    {
        if(a >= 0)
            return static_cast<maxType>(a);
        if(b >= 0)
            return static_cast<maxType>(b);
        return (static_cast<minType>(a) < static_cast<minType>(b)) ?
               static_cast<maxType>(b) : static_cast<maxType>(a);
    }
    return static_cast<maxType>(a) < static_cast<maxType>(b) ?
           static_cast<maxType>(b) : static_cast<maxType>(a);
}

template<typename T,
         typename = std::enable_if_t<std::is_integral<T>::value ||  std::is_pointer<T>::value>>
T to2(T k) // result = 2 ^ k
{
    if(k < 0)
        return 0;
    T a = 1;
    while((a < k) && (a > 0))
        a <<= 1;
    return a;
}

template<typename T,
         typename = std::enable_if_t<std::is_integral<T>::value || std::is_pointer<T>::value>>
constexpr T to_bit_mask(T k) // mask = min(2 ^ x - 1) >= k
{
    if(k < 0)
        return std::numeric_limits<T>::min();
    T mask = 0;
    while(mask < k)
        mask = (mask << 1) + 1;
    return mask;
}

template<typename T,
         typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr unsigned char max_bit_pos(T value)
{
    if(value < 0)
        return type_info<T>::bit_length - static_cast<unsigned char>(1);
    T mask = ~static_cast<T>(0);
    unsigned char ans = 255;
    while(mask & value)
        mask <<= 1, ans++;
    return ans;
}

template<typename T,
        typename = std::enable_if_t<std::is_integral<T>::value>>
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
    typename = std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value>>
constexpr inline T sqr(T value)
{
    return value * value;
}

template<typename T, typename PT = unsigned,
    typename = std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<PT>::value>>
constexpr T pow(T value, PT power)
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

template<typename T,
         typename = std::enable_if_t<std::is_unsigned<T>::value>>
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
		now = !init;
		return result;
	}
};
