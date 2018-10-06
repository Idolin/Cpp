#pragma once

#include "t_useful.hpp"

#include <type_traits>

struct HashableTag
{};

template<typename T = void>
struct is_bit_movable;

template<>
struct is_bit_movable<void>
{};

template<typename T>
struct is_bit_movable
{
    enum {
        value = std::is_pod<T>::value || std::is_pointer<T>::value ||
                std::is_base_of<is_bit_movable<>, T>::value
    };
};

template<typename T = void>
struct is_bit_copyable;

template<>
struct is_bit_copyable<void>
{};

template<typename T>
struct is_bit_copyable
{
    enum
    {
        value = std::is_pod<T>::value || std::is_pointer<T>::value ||
                std::is_base_of<is_bit_copyable<>, T>::value
    };
};

template<typename Self = void>
struct mergeable;

template<>
struct mergeable<void>
{};

template<typename Self>
struct mergeable: mergeable<void>
{
    virtual void merge(Self&& otr) = 0;
};

template<typename T>
struct is_mergeable
{
    enum
    {
        value = std::is_base_of<mergeable<T>, T>::value
    };
};

template<typename T, typename Enable = void>
struct is_less_comparable;

template<typename T>
struct is_less_comparable<T, typename std::enable_if<std::is_integral<T>::value || std::is_pointer<T>::value>::type>
{
    enum {
        value = true
    };
};

template<typename T>
struct is_less_comparable<T, typename std::enable_if<!std::is_integral<T>::value && !std::is_pointer<T>::value>::type>
{
private:
    static uint8_t test(_rank<0>);

    template<typename = decltype(std::declval<typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>() <
                                 std::declval<typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>())>
    static uint16_t test(_rank<1>);

public:
    enum
    {
        value = sizeof(test<>(_rank<1>())) == 2
    };
};

template<typename T>
struct is_hashable
{
    enum
    {
        value = std::is_base_of<HashableTag, T>::value || std::is_integral<T>::value
            || std::is_pointer<T>::value
    };
};
