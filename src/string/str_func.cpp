#include "str_func.h"

unsigned *pi(const str &a)
{
    unsigned *p = new unsigned[a.length()];
    if(a.length() == 0)
        return p;
    p[0] = 0;
    for(unsigned i = 1, k = 0; i < a.length(); i++)
    {
        while((k > 0) && (a[i] != a[k]))
            k = p[k - 1];
        if(a[i] == a[k])
            k++;
        p[i] = k;
    }
    return p;
}

unsigned *z(const str &a)
{
    unsigned *z = new unsigned[a.length()];
    if(a.length() == 0)
        return z;
    z[0] = a.length();
    for(unsigned i = 1, left = 0, right = 1, k; i < a.length(); i++)
    {
        k = 0;
        if(i < right)
            k = (z[i - left] < right - i) ? z[i - left] : right - i;
        if(i + k >= right)
        {
            while((i + k < a.length()) && (a[k] == a[i + k]))
                k++;
            left = i, right = k + i;
        }
        z[i] = k;
    }
    return z;
}

unsigned find(const str &s, const str &sub, const unsigned *const pi_sub)
{
    if(sub.length() == 0)
        return 0;
    for(unsigned i = 0, k = 0; i < s.length(); i++)
    {
        while((k > 0) && (s[i] != sub[k]))
            k = pi_sub[k - 1];
        if(s[i] == sub[k])
            if(++k == sub.length())
                return i + 1 - sub.length();
    }
    return (unsigned) -1;
}

vect<unsigned> findAll(const str &s, const str &sub, const unsigned *const pi_sub)
{
    vect<unsigned> matches;
    if(sub.length() == 0)
    {
        for(unsigned i = 0; i < s.length(); i++)
            matches.push(i);
        return matches;
    }
    for(unsigned i = 0, k = 0; i < s.length(); i++)
    {
        while((k > 0) && (s[i] != sub[k]))
            k = pi_sub[k - 1];
        if(s[i] == sub[k])
            if(++k == sub.length())
            {
                matches.push(i - sub.length() + 1);
                k = pi_sub[k - 1];
            }
    }
    return matches;
}

vect<unsigned> findAllClose(const str, const str, unsigned, const unsigned *);

vect<unsigned> findAllClose(const str, const str, unsigned);