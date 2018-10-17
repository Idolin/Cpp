#pragma once

#include "../container/vector.hpp"

template<typename T>
struct crga
{
    vect <T> *m;
    unsigned len;
    T last;

    crga(unsigned len = 1, T start = 0) : m(new vect<T>(len)), len(len), last(-start)
    {
        m->set(0, start);
        for(unsigned i = 1; i < len; i++)
            m->m[i] = 0;
    }

    ~crga()
    {}

    void set(T x, unsigned start, unsigned end)
    {
        if(end >= len)
        {
            m->set(end, 0);
            m->m[len] = last;
            for(unsigned i = len + 1; i < end; i++)
                m->m[i] = 0;
            len = end + 1;
        }
        if(len == end + 1)
            last = -x;
        else
            m->m[end + 1] -= x;
        m->m[start] += x;
    }

    vect <T> *trans()
    {
        T x = 0;
        for(unsigned i = 0; i < len; i++)
        {
            x += m->m[i];
            m->m[i] = x;
        }
        m->maxs = len;
        return m;
    }
};

template<typename T>
struct crgt
{
    T **m;
    unsigned lenx, leny;

    crgt(unsigned lenx, unsigned leny, T start = 0) : m(new T *[lenx + 1]), lenx(lenx), leny(leny)
    {
        for(unsigned k = 0; k <= lenx; k++)
            m[k] = new T[leny + 1];
        for(unsigned x = 0; x < lenx; x++)
            for(unsigned y = 0; y < leny; y++)
                m[x][y] = 0;
        m[0][0] = start;
    }

    ~crgt()
    {}

    void set(T x, unsigned x1, unsigned y1, unsigned x2, unsigned y2)
    {
        m[x1][y1] += x;
        m[x2 + 1][y1] -= x;
        m[x1][y2 + 1] -= x;
        m[x2 + 1][y2 + 1] += x;
    }

    T **trans()
    {
        for(unsigned x = 0; x < lenx; x++)
        {
            T v = m[x][0];
            for(unsigned y = 0; y < leny; y++)
            {
                m[x + 1][y] += m[x][y];
                m[x][y] = v;
                v += m[x][y + 1];
            }
        }
        return m;
    }
};

template<typename T, typename T2>
struct grf
{
    unsigned len;
    T2 *prefix;

    grf(T *a, unsigned len) : len(len), prefix(new T2[len])
    {
        if(len == 0)
            return;
        prefix[0] = a[0];
        for(unsigned k = 1; k < len; k++)
            prefix[k] = funcU(prefix[k - 1], a[k]);
    }

    ~grf()
    {
        delete[] prefix;
    }

    inline T2 funcU(T2 x1, T x2)
    {
        return x1 + x2;
    }

    inline T2 funcD(T2 x1, T2 x2)
    {
        return x2 - x1;
    }

    T2 get()
    {
        return prefix[len - 1];
    }

    T2 get(unsigned to)
    {
        return prefix[to];
    }

    T2 get(unsigned from, unsigned to)
    {
        if(from == 0)
            return prefix[to];
        return funcD(prefix[from - 1], prefix[to]);
    }
};
