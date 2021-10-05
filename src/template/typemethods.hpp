#pragma once

#include <cstdint>
#include <type_traits>

#include "t_useful.hpp"

template<typename T, typename T2>
struct is_smaller_type
{
    static constexpr bool value = sizeof(T) < sizeof(T2);
};

template<typename T>
struct type_info;

namespace
{
    template<typename T>
    struct _typeSeqBase;

    template<>
    struct _typeSeqBase<signed char>
    {
        typedef signed char type;
        typedef type_info<short> next;
        constexpr static const char *name = "signed char";
        constexpr static const char *specifier = "%hhd";
    };


    template<>
    struct _typeSeqBase<short>
    {
        typedef short type;
        typedef type_info<int> next;
        constexpr static const char *name = "short";
        constexpr static const char *specifier = "%hd";
    };

    template<>
    struct _typeSeqBase<int>
    {
        typedef int type;
        typedef type_info<long> next;
        constexpr static const char *name = "int";
        constexpr static const char *specifier = "%d";
    };

    template<>
    struct _typeSeqBase<long>
    {
        typedef long type;
        typedef type_info<long long> next;
        constexpr static const char *name = "long";
        constexpr static const char *specifier = "%ld";
    };

    template<>
    struct _typeSeqBase<long long>
    {
        typedef long long type;
        constexpr static const char *name = "long long";
        constexpr static const char *specifier = "%lld";
    };

    template<>
    struct _typeSeqBase<unsigned char>
    {
        typedef unsigned char type;
        typedef type_info<unsigned short> next;
        constexpr static const char *name = "unsigned char";
        constexpr static const char *specifier = "%hhu";
    };

    template<>
    struct _typeSeqBase<unsigned short>
    {
        typedef unsigned short type;
        typedef type_info<unsigned> next;
        constexpr static const char *name = "unsigned short";
        constexpr static const char *specifier = "%hu";
    };

    template<>
    struct _typeSeqBase<unsigned>
    {
        typedef unsigned type;
        typedef type_info<unsigned long> next;
        constexpr static const char *name = "unsigned int";
        constexpr static const char *specifier = "%u";
    };

    template<>
    struct _typeSeqBase<unsigned long>
    {
        typedef unsigned long type;
        typedef type_info<unsigned long long> next;
        constexpr static const char *name = "unsigned long";
        constexpr static const char *specifier = "%lu";
    };

    template<>
    struct _typeSeqBase<unsigned long long>
    {
        typedef unsigned long long type;
        constexpr static const char *name = "unsigned long long";
        constexpr static const char *specifier = "%llu";
    };

    template<>
    struct _typeSeqBase<char>
    {
        typedef char type;
        typedef std::conditional_t<std::is_signed<char>::value,
                type_info<short>,
                type_info<unsigned short>> next;
        constexpr static const char *name = "char";
        constexpr static const char *specifier = "%c";
    };

    static constexpr unsigned char get_pwr2(unsigned char type_size)
    {
        unsigned char b = 0;
        while(type_size)
            b++, type_size >>= 1;
        return b + 2;
    }
}

template<typename T>
struct type_info
{
    typedef T type;
    static constexpr const char *name = _typeSeqBase<T>::name;
    static constexpr const char *specifier = _typeSeqBase<T>::specifier;
    static constexpr unsigned char bit_length = sizeof(T) << 3;
    static constexpr unsigned char pwr2_length = get_pwr2(sizeof(T));
};

template<typename T, typename T2>
struct get_smaller_type
{
    typedef std::conditional_t<is_smaller_type<T, T2>::value, T, T2> type;
};

template<typename T, typename T2>
struct get_bigger_type
{
    typedef std::conditional_t<is_smaller_type<T, T2>::value, T2, T> type;
};

template<typename T, typename Enable = void>
struct next_bigger_integral_type
{};

template<typename T>
struct next_bigger_integral_type<T, typename std::enable_if<
        is_smaller_type<T, typename type_info<T>::next::type>::value>::type>
{
    typedef typename type_info<T>::next::type type;
};

template<typename T>
struct next_bigger_integral_type<T, typename std::enable_if<
        !is_smaller_type<T, typename type_info<T>::next::type>::value>::type>
{
    typedef typename next_bigger_integral_type<typename type_info<T>::next::type>::type type;
};

template <typename T>
struct has_next_bigger_integral_type
{
private:
    template<typename Enabled = void>
    static uint8_t test(rank_p<0>);

    template<typename type_info<T>::next::type = 0>
    static uint16_t test(rank_p<1>);

public:
    static constexpr bool value = (sizeof(test<>(rank_p<1>())) == 2);
};

template<typename T, typename Enable = void>
struct next_bigger_integral_type_is_exists;

template<typename T>
struct next_bigger_integral_type_is_exists<T, typename std::enable_if<!has_next_bigger_integral_type<T>::value>::type>
{
    typedef T type;
};

template<typename T>
struct next_bigger_integral_type_is_exists<T, typename std::enable_if<
        is_smaller_type<T, typename type_info<T>::next::type>::value>::type>
{
    typedef typename type_info<T>::next::type type;
};

template<typename T>
struct next_bigger_integral_type_is_exists<T, typename std::enable_if<
        !is_smaller_type<T, typename type_info<T>::next::type>::value>::type>
{
    typedef typename next_bigger_integral_type_is_exists<typename type_info<T>::next::type>::type type;
};

template<typename T, bool sign>
struct change_signedness
{
    typedef std::conditional_t<sign, std::make_signed_t<T>, std::make_unsigned_t<T>> type;
};

template<typename T>
struct clear_type
{
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template<typename T, typename Enable = void>
struct def_get_by
{
    typedef const T& type;
};

template<typename T>
struct def_get_by<T, typename std::enable_if<
        std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_pointer<T>::value>::type>
{
    typedef T type;
};

template<typename T>
using def_get_by_t = typename def_get_by<T>::type;

template<typename T>
struct compare_func
{
    typedef bool (*type)(def_get_by_t<T>, def_get_by_t<T>);
};

template<typename T>
using compare_func_t = typename compare_func<T>::type;


template<typename T, typename R>
constexpr inline unsigned short getter_parts()
{
    return (sizeof(T) + sizeof(R) - 1) / sizeof(R);
};

// T - any type, R - unsigned, returns from lowest (least matters) parts to highest (most matters) parts
template<typename T, typename R = uint8_t,
    typename = typename std::enable_if<std::is_unsigned<R>::value>::type>
struct getter_func
{
    typedef R (*type)(def_get_by_t<T>, unsigned short);

    typedef R RType;

    constexpr static const unsigned short parts = getter_parts<T, R>();
};
