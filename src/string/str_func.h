#pragma once

#include "../container/vector.hpp"
#include "str.h"

unsigned *pi(const str &);

unsigned *z(const str &);

unsigned find(const str &, const str &, const unsigned *const);

inline unsigned find(const str &s, const str &sub)
{
    return find(s, sub, pi(sub));
}

inline bool isSubstr(const str &s, const str &sub, const unsigned *const pi_sub)
{
    return (find(s, sub, pi_sub));
}

inline bool isSubstr(const str &s, const str &sub)
{
    return (find(s, sub));
}

vect<unsigned> findAll(const str &, const str &, const unsigned *const);

inline vect<unsigned> findAll(const str &s, const str &sub)
{
    return findAll(s, sub, pi(sub));
}

vect<unsigned> findAllClose(const str &, const str &, unsigned, const unsigned *const);

vect<unsigned> findAllClose(const str &, const str &, unsigned);

inline unsigned getPeriod(const unsigned *const pi, unsigned len)
{
    return pi[len - 1];
}

inline unsigned getPeriod(const str &s)
{
    return getPeriod(pi(s), s.length());
}
