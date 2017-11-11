#pragma once

#include "../other/arraymethods.hpp"
#include "../other/defdef.h"
#include "../debug/def_debug.h"

#include <algorithm>
#include <stdio.h>

#ifdef COUNTSWAPS
extern unsigned long long swapscounter;
#endif // COUNTSWAPS

template<typename T>
struct vect
{
protected:
    T *m;
public:
    unsigned size, maxs;

    vect(unsigned size = 4) : m(new T[size]), size(size), maxs(0)
    {}

    vect(vect const &f) : m(new T[f.size]), size(f.size), maxs(f.maxs)
    {
        _copy(m, size, f.m);
    }

    vect &operator=(vect const &f)
    {
        if(this != &f)
            delete[] m;
        maxs = f.maxs;
        size = f.size;
        m = new T[size];
        _copy(m, size, f.m);
        return *this;
    }

    ~vect()
    {
        delete[] m;
    }

    operator T *()
    {
        T *r = new T[maxs];
        _copy(r, maxs, m);
        return r;
    }

    T &operator[](unsigned index)
    {
        if(index >= size)
        {
            if(size == 0)
                this->resizeTo(index + 1);
            else
            {
                int k = 0;
                while(index >= size << ++k);
                this->resizeUp(k);
            }
        }
        if(index >= maxs)
            maxs = index + 1;
        return m[index];
    }

    const T &operator[](unsigned index) const
    {
        ASSERT(index < size, "Vector: index out of range");
        return m[index];
    }

    void resizeTo(unsigned k)
    {
        T *m2 = new T[k];
        _copy(m2, size, m);
        delete[] m;
        m = m2;
        size = k;
    }

    void resizeUp(unsigned k = 1)
    {
        unsigned newsize = (size + (size == 0)) * (1 << k);
        T *m2 = new T[newsize];
        _copy(m2, size, m);
        delete[] m;
        m = m2;
        size = newsize;
    }

    T back()
    {
        ASSERT(maxs > 0);
        return m[maxs - 1];
    }

    void push(T x)
    {
        if(maxs == size)
            this->resizeUp();
        m[maxs++] = x;
    }

    void swap(unsigned first, unsigned second)
    {
        ASSERT((first < size) && (second < size));
#ifdef COUNTSWAPS
        swapscounter++;
#endif // COUNTSWAPS
        std::swap(m + first, m + second);
    }

    void display(unsigned start = 0, unsigned end = (unsigned) -1, const char *prf = "%u", const char *del = " ")
    {
        if(maxs == 0)
            fputs("Vector is empty\n", stdout);
        else
        {
            smin_(end, maxs);
            _display(m + start, end - start, prf, del);
        }
    }
};

template<typename T>    //error code = 2
struct cvect : vect<T>
{
    T *m;
    unsigned maxs, size, resizekf;

    cvect(unsigned size = 20, unsigned resizekf = 3) : m(new T[size]), maxs(0), size(size), resizekf(resizekf)
    {}

    cvect(cvect const &f) : m(new T[f.size]), maxs(f.maxs), size(f.size), resizekf(f.resizekf)
    {
        _copy(m, size, f.m);
    }

    cvect &operator=(cvect const &f)
    {
        if(this != &f)
            delete[] m;
        maxs = f.maxs;
        size = f.size;
        resizekf = f.resizekf;
        m = new T[size];
        _copy(m, size, f.m);
        return *this;
    }

    ~cvect()
    {
        delete[] m;
    }

    void resizeUp(unsigned k)
    {
        T *m2 = new T[size *= k];
        _copy(m2, size, m);
        delete[] m;
        m = m2;
    }

    void resizeTo(unsigned k)
    {
        T *m2 = new T[k];
        _copy(m2, size < k ? size : k, m);
        delete[] m;
        m = m2;
        size = k;
    }

    T getMax()
    {
        if(maxs == 0)
            throw 2;
        return _max(m, maxs);
    }

    T getMin()
    {
        if(maxs == 0)
            throw 2;
        return _min(m, maxs);
    }

    unsigned getMinPos()
    {
        if(maxs == 0)
            throw 2;
        return _minInd(m, maxs);
    }

    unsigned getMaxPos()
    {
        if(maxs == 0)
            throw 2;
        return _maxInd(m, maxs);
    }

    template<typename T2>
    T2 sum()
    {
        return _sum<T, T2>(m, maxs);
    }

    void set(unsigned p, T x)
    {
        if(p >= size)
        {
            if(resizekf > 1 and size > 0)
            {
                unsigned k = resizekf;
                while(p >= size * k)
                    k *= resizekf;
                this->resizeUp(k);
            } else
                this->resizeTo(p + 1);
        }
        if(p >= maxs)
            maxs = p + 1;
        m[p] = x;
    }

    void putLast(T x)
    {
        if(maxs == size)
        {
            if(resizekf > 1 and size > 0)
                this->resizeUp(resizekf);
            else
                this->resizeTo(size + 1);
        }
        m[maxs++] = x;
    }

    bool checksorted(bool smallToBig = true, unsigned start = 0, unsigned end = -1)
    {
        if(maxs == 0)
            return true;
        if(end >= maxs)
            end = maxs - 1;
        for(unsigned t = start; t < end; t++)
            if(m[t] != m[t + 1] and (smallToBig xor _more(m[t + 1], m[t])))
                return false;
        return true;
    }
};

//TODO
template<typename T>
struct pvect
{
};
