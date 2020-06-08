#pragma once

#include "str.h"

inline unsigned getPeriod(const unsigned *const pi, unsigned len)
{
    return len - pi[len - 1];
}

inline unsigned getPeriod(const str &s)
{
    return getPeriod(pi(s), s.length());
}
