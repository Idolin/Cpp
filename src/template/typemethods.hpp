#pragma once

#include <cstdint>
#include <type_traits>

#include "t_useful.hpp"

template<typename T, typename T2>
struct _typeLower
{
    enum {
        value = sizeof(T) < sizeof(T2)
    };
};

template<typename T>
struct _typeSeq;

namespace
{
    template<typename T>
    struct _typeSeqBase;

    template<>
    struct _typeSeqBase<signed char>
    {
        typedef signed char type;
        typedef _typeSeq<short> next;
        constexpr static const char *name = "signed char";
        constexpr static const char *specifier = "%hhd";
    };


    template<>
    struct _typeSeqBase<short>
    {
        typedef short type;
        typedef _typeSeq<int> next;
        constexpr static const char *name = "short";
        constexpr static const char *specifier = "%hd";
    };

    template<>
    struct _typeSeqBase<int>
    {
        typedef int type;
        typedef _typeSeq<long> next;
        constexpr static const char *name = "int";
        constexpr static const char *specifier = "%d";
    };

    template<>
    struct _typeSeqBase<long>
    {
        typedef long type;
        typedef _typeSeq<long long> next;
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
        typedef _typeSeq<unsigned short> next;
        constexpr static const char *name = "unsigned char";
        constexpr static const char *specifier = "%hhu";
    };

    template<>
    struct _typeSeqBase<unsigned short>
    {
        typedef unsigned short type;
        typedef _typeSeq<unsigned> next;
        constexpr static const char *name = "unsigned short";
        constexpr static const char *specifier = "%hu";
    };

    template<>
    struct _typeSeqBase<unsigned>
    {
        typedef unsigned type;
        typedef _typeSeq<unsigned long> next;
        constexpr static const char *name = "unsigned int";
        constexpr static const char *specifier = "%u";
    };

    template<>
    struct _typeSeqBase<unsigned long>
    {
        typedef unsigned long type;
        typedef _typeSeq<unsigned long long> next;
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
        typedef typename std::conditional<std::is_signed<char>::value,
                _typeSeq<short>,
                _typeSeq<unsigned short>>::type next;
        constexpr static const char *name = "char";
        constexpr static const char *specifier = "%c";
    };

    constexpr static unsigned char get_pwr2(unsigned char type_size)
    {
        unsigned char b = 0;
        while(type_size)
            b++, type_size >>= 1;
        return b + 2;
    }
}

template<typename T>
struct _typeSeq
{
    typedef T type;
    constexpr static const char *name = _typeSeqBase<T>::name;
    constexpr static const char *specifier = _typeSeqBase<T>::specifier;
    constexpr static unsigned char bit_length = sizeof(T) << 3;
    constexpr static unsigned char pwr2_length = get_pwr2(sizeof(T));
};

template<typename T, typename T2>
struct _getMinType
{
    typedef typename std::conditional<_typeLower<T, T2>::value, T, T2>::type type;
};

template<typename T, typename T2>
struct _getMaxType
{
    typedef typename std::conditional<_typeLower<T, T2>::value, T2, T>::type type;
};

template<typename T, typename Enable = void>
struct _getNextType
{};

template<typename T>
struct _getNextType<T, typename std::enable_if<
        _typeLower<T, typename _typeSeq<T>::next::type>::value>::type>
{
    typedef typename _typeSeq<T>::next::type type;
};

template<typename T>
struct _getNextType<T, typename std::enable_if<
        !_typeLower<T, typename _typeSeq<T>::next::type>::value>::type>
{
    typedef typename _getNextType<typename _typeSeq<T>::next::type>::type type;
};

template <typename T>
struct _hasNextType
{
private:
    template<typename Enabled = void>
    static uint8_t test(_rank<0>);

    template<typename _typeSeq<T>::next::type = 0>
    static uint16_t test(_rank<1>);

public:
    enum
    {
        value = sizeof(test<>(_rank<1>())) == 2
    };
};

template<typename T, typename Enable = void>
struct _getNextTypeIfExists;

template<typename T>
struct _getNextTypeIfExists<T, typename std::enable_if<!_hasNextType<T>::value>::type>
{
    typedef T type;
};

template<typename T>
struct _getNextTypeIfExists<T, typename std::enable_if<
        _typeLower<T, typename _typeSeq<T>::next::type>::value>::type>
{
    typedef typename _typeSeq<T>::next::type type;
};

template<typename T>
struct _getNextTypeIfExists<T, typename std::enable_if<
        !_typeLower<T, typename _typeSeq<T>::next::type>::value>::type>
{
    typedef typename _getNextTypeIfExists<typename _typeSeq<T>::next::type>::type type;
};

template<typename T, bool sign>
struct _changeSigned
{
    typedef typename std::conditional<sign, typename std::make_signed<T>::type,
            typename std::make_unsigned<T>::type>::type type;
};

template<typename T>
struct clear_type
{
    typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
};

template<typename T, typename Enable = void>
struct def_get_by_value
{};

template<typename T>
struct def_get_by_value<T, typename std::enable_if<
        std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_pointer<T>::value>::type>
{
    typedef T type;
};

template<typename T, typename Enable = void>
struct def_get_by_reference
{};

template<typename T>
struct def_get_by_reference<T, typename std::enable_if<
        !std::is_integral<T>::value && !std::is_floating_point<T>::value && !std::is_pointer<T>::value>::type>
{
    typedef T type;
};

template<typename T, typename Enable = void>
struct def_get_by
{};

template<typename T>
struct def_get_by<T, typename std::enable_if<
        std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_pointer<T>::value>::type>
{
    typedef T type;
};

template<typename T>
struct def_get_by<T, typename std::enable_if<
        !std::is_integral<T>::value && !std::is_floating_point<T>::value && !std::is_pointer<T>::value>::type>
{
    typedef const T& type;
};

template<typename T>
struct compare_func
{
    typedef bool (*type)(typename def_get_by<T>::type, typename def_get_by<T>::type);
};

template<typename T, typename R>
constexpr inline unsigned short getter_parts()
{
    return (sizeof(T) + sizeof(R) - 1) / sizeof(R);
};

//T - any type, R - unsigned, returns from lowest(least matters) parts to highest(most matters) parts
template<typename T, typename R = uint8_t,
    typename = typename std::enable_if<std::is_unsigned<R>::value>::type>
struct getter_func
{
    typedef R (*type)(typename def_get_by<T>::type, unsigned short);

    typedef R RType;

    constexpr static const unsigned short parts = getter_parts<T, R>();
};
