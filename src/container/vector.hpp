#pragma once

#include "../other/arraymethods.hpp"
#include "../other/defdef.h"
#include "../debug/def_debug.h"
#include "../other/displaymethods.hpp"

#include <algorithm>
#include <stdio.h>

template<typename T>
struct vect
{
protected:
    T *m;
public:
    unsigned size, maxs;

    vect(unsigned size = 4): m(new T[size]), size(size), maxs(0)
    {}

    vect(vect const &f): m(new T[f.size]), size(f.size), maxs(f.maxs)
    {
        _copy(m, size, f.m);
    }

    vect(vect&& f): m(f.m), size(f.size), maxs(f.maxs)
    {
        f.m = nullptr;
        f.size = 0;
        f.maxs = 0;
    }

    vect& operator=(vect const &f)
    {
        if(this != &f)
        {
            delete[] m;
            maxs = f.maxs;
            size = f.size;
            m = new T[size];
            _copy(m, size, f.m);
        }
        return *this;
    }

    vect& operator=(vect&& f)
    {
        if(this != &f)
        {
            delete[] m;
            m = f.m;
            size = f.size;
            maxs = f.maxs;
            f.m = nullptr;
            f.size = 0;
            f.maxs = 0;
        }
        return *this;
    }

    ~vect()
    {
        delete[] m;
    }

    operator T*()
    {
        T *r = new T[maxs];
        _copy(r, maxs, m);
        return r;
    }

    T& operator[](unsigned index)
    {
        if(index >= size)
        {
            if(maxs == 0)
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
        ASSERT(index < size, "Vector: index %u out of range(size: %u)", index, size);
        DEBUGIFMSG(index >= maxs, "Vector: value at index %u not set", index);
        return m[index];
    }

    void resizeTo(unsigned k)
    {
        DEBUGLVLIFMSG(3, k < maxs, "new size smaller than index of last element, some elements will be deleted!");
        _resize(m, size, k);
        size = k;
    }

    void resizeUp(unsigned k = 1)
    {
        unsigned oldsize = size;;
        m = _resize(m, oldsize, size = (oldsize + (oldsize == 0)) * (1 << k));
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
        ASSERT((first < size) && (second < size), "Vector: attempt to swap values "
                "out of range(size: %u, first index: %u, second index: %u)", size, first, second);
        std::swap(m + first, m + second);
    }

    T getMax()
    {
        ASSERT(maxs > 0);
        return _max(m, maxs);
    }

    T getMin()
    {
        ASSERT(maxs > 0);
        return _min(m, maxs);
    }

    unsigned getMinPos()
    {
        ASSERT(maxs > 0);
        return _minInd(m, maxs);
    }

    unsigned getMaxPos()
    {
        ASSERT(maxs > 0);
        return _maxInd(m, maxs);
    }

    template<typename T2>
    T2 sum()
    {
        return _sum<T, T2>(m, maxs);
    }

    template<bool (*compare)(const T &, const T &) = _less<T>>
    bool checksorted(unsigned start = 0, unsigned end = std::numeric_limits<unsigned>::max())
    {
        smin_(end, maxs);
        return _checksorted<T, compare>(m + start, m + end);
    }

    template<void (*show)(const T&) = &_tshow>
    void display(unsigned start = 0, unsigned end = std::numeric_limits<unsigned>::max(), const char* del = ", ")
    {
        if(maxs == 0)
            puts("Vector is empty");
        else
        {
            smin_(end, maxs);
            _tdisplay<T, show>(m + start, end - start, del);
        }
    }
};

//TODO
template<typename T>
struct pvect
{};
