#pragma once

#include "../template/typemethods.hpp"

#define elif else if

template<typename T>
inline void set_min(T &a, typename def_get_by<T>::type b)
{
    if(b < a)
        a = b;
}

template<typename T>
inline void set_max(T &a, typename def_get_by<T>::type b)
{
    if(a < b)
        a = b;
}
