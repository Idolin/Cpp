#pragma once

#include "../template/arraymethods.hpp"
#include "../template/commonmethods.hpp"
#include "../template/typemethods.hpp"
#include "../template/struct_tags.hpp"
#include "sortsquare.hpp"
#include <functional>

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
constexpr static typename getter_func<T>::RType def_getter(T value, unsigned short part)
{
    typedef typename getter_func<T>::RType RType;

    return (std::is_signed<T>::value && (part == 0)) ?
           static_cast<RType>(static_cast<typename std::make_unsigned<T>::type>(value) >> ((sizeof(T) - sizeof(RType)) * 8)) ^
               (static_cast<RType>(1 << (sizeof(RType) * 8 - 1))) :
           static_cast<RType>(value >> (sizeof(T) + part * sizeof(RType) - sizeof(RType)) * 8);
}

template<typename RType, typename RNow, typename T,
        typename getter_func<T, RNow>::type get, unsigned short parts,
        typename Enable = void>
struct make_getter;

template<typename RType, typename RNow, typename T,
        typename getter_func<T, RNow>::type get, unsigned short parts>
struct make_getter<RType, RNow, T, get, parts, typename std::enable_if<
        std::is_unsigned<RType>::value && (sizeof(RType) == sizeof(RNow)) &&
        (sizeof(RType) <= 2) && std::is_unsigned<RNow>::value>::type>
{
    static auto getter() -> RType(*)(T, unsigned short)
    {
        return get;
    }

    constexpr static unsigned short parts_new = parts;
};

template<typename RType, typename RNow, typename T,
        typename getter_func<T, RNow>::type get, unsigned short parts>
struct make_getter<RType, RNow, T, get, parts, typename std::enable_if<
        std::is_unsigned<RType>::value && (sizeof(RType) != sizeof(RNow)) &&
        (sizeof(RNow) == 1) &&
        (sizeof(RType) <= 2) && std::is_unsigned<RNow>::value>::type>
{
    static auto getter() -> RType(*)(T, unsigned short)
    {
        return [](T value, unsigned short part) -> RType {
            unsigned short partFrom = part * sizeof(RType);
            RType ans = get(value, partFrom);
            unsigned short partTo = partFrom + sizeof(RType);
            if(partTo > parts)
                for(unsigned short i = partFrom + 1; i < partTo; i++)
                    ans = (ans << 8) | get(value, i);
            else
                for(unsigned short i = 1; i < sizeof(RType); i++)
                    ans = (ans << 8) | get(value, partFrom + i);
            return ans;
        };
    }

    constexpr static unsigned short parts_new = (parts + sizeof(RType) - 1) / sizeof(RType);
};

template<typename RType, typename RNow, typename T,
        typename getter_func<T, RNow>::type get, unsigned short parts>
struct make_getter<RType, RNow, T, get, parts, typename std::enable_if<
        std::is_unsigned<RType>::value && (sizeof(RType) != sizeof(RNow)) &&
        (sizeof(RNow) != 1) &&
        (sizeof(RType) <= 2) && std::is_unsigned<RNow>::value>::type>
{
    static auto getter() -> RType(*)(T, unsigned short)
    {
        return [](T value, unsigned short part) -> RType {
            constexpr unsigned char shift = min_bit_pos(sizeof(RNow) >> (sizeof(RType) - 1));
            constexpr unsigned short p_mask = (1 << shift) - 1;
            return static_cast<RType>(get(value, part >> shift) >> ((part & p_mask) * 8));
        };
    }

    constexpr static unsigned short parts_new = parts * sizeof(RNow) / sizeof(RType);
};

template<typename T, typename R, typename getter_func<T, R>::type getter,
        unsigned short parts,
        typename compare_func<T>::type compare, typename E>
static void bucketsort_impl(T *start, T *end, unsigned short from = 0,
                            T *copy_array = nullptr)
{
    typedef typename std::conditional<(sizeof(R) * parts == 1), uint8_t, uint16_t>::type GR;
    typename getter_func<T, GR>::type get_f = make_getter<GR, R, T, getter, parts>::getter();
    unsigned short parts_new = make_getter<GR, R, T, getter, parts>::parts_new;

    E length = end - start;
    E *items_in_bucket = new E[std::numeric_limits<GR>::max() + 1];
    _fill(items_in_bucket, std::numeric_limits<GR>::max() + 1);
    const E index_to = (sizeof(E) == sizeof(GR)) ? 0 :
                       static_cast<E>(std::numeric_limits<GR>::max()) + 1;

    for(E i = 0;i < length;i++)
        items_in_bucket[get_f(start[i], from)]++;
    for(E i = 1;i != index_to;i++)
        items_in_bucket[i] += items_in_bucket[i - 1];
    T *copy = (copy_array) ? copy_array : new T[length];
    _move(copy, length, start);
    for(E i = 0;i < length;i++)
        start[--items_in_bucket[get_f(copy[i], from)]] = std::move(copy[i]);
    if(from < parts_new - 1)
    {
        if(compare)
            for(E i = 1; i != index_to; i++)
                if(items_in_bucket[i] - items_in_bucket[i - 1] < 256)
                    minmaxsort<T, compare>(start + items_in_bucket[i - 1],
                                           start + items_in_bucket[i]);
                else
                    bucketsort_impl<T, R, getter, parts, compare, E>(
                            start + items_in_bucket[i - 1], start + items_in_bucket[i],
                            from + 1, copy);
        else
            for(E i = 1; i != index_to; i++)
                if(items_in_bucket[i] - items_in_bucket[i - 1] > 1)
                    bucketsort_impl<T, R, getter, parts, compare, E>(
                            start + items_in_bucket[i - 1], start + items_in_bucket[i],
                            from + 1, copy);
    }
    delete[] items_in_bucket;
    if(!(copy_array))
        delete[] copy;
}

template<typename T, typename R = typename getter_func<T>::RType,
        typename getter_func<T, R>::type getter = def_getter<T>,
        unsigned short parts = getter_func<T, R>::parts,
        typename compare_func<T>::type compare = _less<T>>
void bucketsort(T *start, T *end)
{
    if(end - start < ((1 << 16) - 1))
        bucketsort_impl<T, R, getter, parts, compare, uint16_t>(start, end);
    else
        bucketsort_impl<T, R, getter, parts, compare, u_int32_t>(start, end);
}