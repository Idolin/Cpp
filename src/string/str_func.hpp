#pragma once

#include "../container/vector.hpp"

#include <cstddef>
#include <limits>

template<typename T>
std::size_t *pi(const T* const s, std::size_t length)
{
    std::size_t *p = new std::size_t[length];
    if(length == 0)
        return p;
    p[0] = 0;
    for(std::size_t i = 1, k = 0; i < length; i++)
    {
        while((k > 0) && (s[i] != s[k]))
            k = p[k - 1];
        if(s[i] == s[k])
            k++;
        p[i] = k;
    }
    return p;
}

template<typename T>
std::size_t *z(const T* const s, std::size_t length)
{
    std::size_t *z = new std::size_t[length];
    if(length == 0)
        return z;
    z[0] = length;
    for(size_t i = 1, left = 0, right = 1, k; i < length; i++)
    {
        k = 0;
        if(i < right)
            k = (z[i - left] < right - i) ? z[i - left] : right - i;
        if(i + k >= right)
        {
            while((i + k < length) && (s[k] == s[i + k]))
                k++;
            left = i, right = k + i;
        }
        z[i] = k;
    }
    return z;
}

template<typename T>
size_t find(const T* const s, std::size_t s_length, const T* const sub, std::size_t sub_length, 
            const std::size_t *const pi_sub = nullptr)
{
    if(sub.length() == 0)
        return 0;
    bool pi_given = (pi_sub != nullptr);
    if(!pi_given)
        pi_sub = pi(sub, sub_length);
    for(size_t i = 0, k = 0; i < s_length; i++)
    {
        while((k > 0) && (s[i] != sub[k]))
            k = pi_sub[k - 1];
        if(s[i] == sub[k])
            if(++k == sub_length)
            {
                if(!pi_given)
                    delete[] pi_sub;
                return i + 1 - sub_length;
            }
    }
    return std::numeric_limits<std::size_t>::max();
}

template<typename T>
vect<std::size_t> findAll(const T* const s, std::size_t s_length, const T* const sub, std::size_t sub_length, 
                     const T* const pi_sub = nullptr)
{
    vect<std::size_t> matches;
    if(sub.length() == 0)
    {
        for(std::size_t i = 0; i < s_length; i++)
            matches.push(i);
        return matches;
    }
    bool pi_given = (pi_sub != nullptr);
    if(!pi_given)
        pi_sub = pi(sub, sub_length);
    for(std::size_t i = 0, k = 0; i < s_length; i++)
    {
        while((k > 0) && (s[i] != sub[k]))
            k = pi_sub[k - 1];
        if(s[i] == sub[k])
            if(++k == sub_length)
            {
                matches.push(i - sub_length + 1);
                k = pi_sub[k - 1];
            }
    }
    if(!pi_given)
        delete[] pi_sub;
    return matches;
}
