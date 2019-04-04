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


#define STRUCT_IS(name, ...) \
    template<typename T, typename Enable = void> \
    struct is_ ## name \
    { \
        enum \
        { \
            value = false \
        }; \
    }; \
    \
    template<typename T> \
    struct is_ ## name<T, typename std::enable_if_t<__VA_ARGS__>> \
    { \
        enum \
        { \
            value = true \
        }; \
    };

STRUCT_IS(less_comparable, std::is_same<decltype(std::declval<typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>() <
                                 std::declval<typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>()), bool>::value)

STRUCT_IS(dereferencable, std::is_lvalue_reference<decltype(*std::declval<typename std::add_lvalue_reference<T>::type>())>::value)

template<typename T>
struct is_hashable
{
    enum
    {
        value = std::is_base_of<HashableTag, T>::value || std::is_integral<T>::value
            || std::is_pointer<T>::value
    };
};
