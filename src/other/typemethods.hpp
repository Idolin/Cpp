#pragma once

#include <cstdint>
#include <type_traits>

#include "../template/t_useful.hpp"

template<typename T, typename T2>
struct _typeLower
{
    enum {
        value = sizeof(T) < sizeof(T2)
    };
};

template<typename T>
struct _typeSeq;

template<>
struct _typeSeq<signed char>
{
    typedef signed char type;
    typedef _typeSeq<short> next;
    constexpr static const char *name = "signed char";
    constexpr static const char *specifier = "%hhd";
};

template<>
struct _typeSeq<short>
{
    typedef short type;
    typedef _typeSeq<int> next;
    constexpr static const char *name = "short";
    constexpr static const char *specifier = "%hd";
};

template<>
struct _typeSeq<int>
{
    typedef int type;
    typedef _typeSeq<long> next;
    constexpr static const char *name = "int";
    constexpr static const char *specifier = "%d";
};

template<>
struct _typeSeq<long>
{
    typedef long type;
    typedef _typeSeq<long long> next;
    constexpr static const char *name = "long";
    constexpr static const char *specifier = "%ld";
};

template<>
struct _typeSeq<long long>
{
    typedef long long type;
    constexpr static const char *name = "long long";
    constexpr static const char *specifier = "%lld";
};

template<>
struct _typeSeq<unsigned char>
{
    typedef unsigned char type;
    typedef _typeSeq<unsigned short> next;
    constexpr static const char *name = "unsigned char";
    constexpr static const char *specifier = "%hhu";
};

template<>
struct _typeSeq<unsigned short>
{
    typedef unsigned short type;
    typedef _typeSeq<unsigned> next;
    constexpr static const char *name = "unsigned short";
    constexpr static const char *specifier = "%hu";
};

template<>
struct _typeSeq<unsigned>
{
    typedef unsigned type;
    typedef _typeSeq<unsigned long> next;
    constexpr static const char *name = "unsigned int";
    constexpr static const char *specifier = "%u";
};

template<>
struct _typeSeq<unsigned long>
{
    typedef unsigned long type;
    typedef _typeSeq<unsigned long long> next;
    constexpr static const char *name = "unsigned long";
    constexpr static const char *specifier = "%lu";
};

template<>
struct _typeSeq<unsigned long long>
{
    typedef unsigned long long type;
    constexpr static const char *name = "unsigned long long";
    constexpr static const char *specifier = "%llu";
};

template<>
struct _typeSeq<char>
{
    typedef char type;
    typedef typename std::conditional<std::is_signed<char>::value,
            _typeSeq<short>,
            _typeSeq<unsigned short>>::type next;
    constexpr static const char *name = "char";
    constexpr static const char *specifier = "%c";
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
    template <typename C>
    static long long test(_rank<0>);

    template <typename C, typename _typeSeq<C>::next::type = 0>
    static char test(_rank<1>);

public:
    enum
    {
        value = (sizeof(test<T>(_rank<1>())) == sizeof(char))
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

//the deadlock empire