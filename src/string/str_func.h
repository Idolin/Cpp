#pragma once

#include "../container/vector.hpp"
#include "str.h"

inline bool isSubstr(const str &s, const str &sub, const unsigned *const pi_sub)
{
    return (find(s, sub, pi_sub));
}

inline bool isSubstr(const str &s, const str &sub)
{
    return (find(s, sub));
}

inline unsigned getPeriod(const unsigned *const pi, unsigned len)
{
    return len - pi[len - 1];
}

inline unsigned getPeriod(const str &s)
{
    return getPeriod(pi(s), s.length());
}
