#pragma once

#include "t_useful.hpp"

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
    enum {
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
    enum {
        value = std::is_base_of<mergeable<T>, T>::value
    };
};