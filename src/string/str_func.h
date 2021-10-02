#pragma once

#include "./cstr.h"
#include "./func.hpp"

inline unsigned getPeriod(const unsigned *const pi, unsigned len)
{
    return len - pi[len - 1];
}

inline unsigned getPeriod(const cstr &s)
{
    return getPeriod(pi(s, s.length()), s.length());
}
