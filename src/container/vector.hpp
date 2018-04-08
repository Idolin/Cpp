#pragma once

#include "../template/arraymethods.hpp"
#include "../other/defdef.h"
#include "../debug/def_debug.h"
#include "../template/displaymethods.hpp"

#include <algorithm>
#include <iterator>
#include <stdio.h>

template<typename T>
struct vect
{
    struct iterator: public std::iterator<std::random_access_iterator_tag, T>
    {
        friend struct vect<T>;

    private:
        T *ptr;

    public:
        iterator(T *ptr): ptr(ptr)
        {}

        iterator(const iterator& otr): ptr(otr.ptr)
        {}

        iterator (iterator&& otr): ptr(otr.ptr)
        {
            otr.ptr = nullptr;
        }

        iterator& operator=(const iterator& otr)
        {
            ptr = otr.ptr;
        }

        iterator& operator=(iterator&& otr)
        {
            if(this != &otr)
            {
                ptr = otr.ptr;
                otr.ptr = nullptr;
            }
        }

        bool operator==(const iterator& otr) const
        {
            return (ptr == otr.ptr);
        }

        bool operator!=(const iterator& otr) const
        {
            return !(*this == otr);
        }

        bool operator<(const iterator& otr) const
        {
            return ptr < otr.ptr;
        }

        bool operator<=(const iterator& otr) const
        {
            return ptr <= otr.ptr;
        }

        bool operator>(const iterator& otr) const
        {
            return ptr > otr.ptr;
        }

        bool operator>=(const iterator& otr) const
        {
            return ptr >= otr.ptr;
        }

        iterator& operator++()
        {
            ptr++;
            return *this;
        }

        iterator operator++(int)
        {
            iterator copy = *this;
            ++*this;
            return copy;
        }

        iterator& operator--()
        {
            ptr--;
            return *this;
        }

        iterator operator--(int)
        {
            iterator copy = *this;
            --*this;
            return copy;
        }

        iterator& operator+=(unsigned long p)
        {
            ptr += p;
            return *this;
        }

        iterator& operator-=(unsigned long p)
        {
            ptr -= p;
            return *this;
        }

        T& operator[](unsigned long index)
        {
            return ptr[index];
        }

        T& operator*()
        {
            return *ptr;
        }

        T *operator->()
        {
            return ptr;
        }
    };

protected:
    T *m;
public:
    unsigned long size, maxs;

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
            m = _new_copy(f.m, size);
        }
        return *this;
    }

    vect& operator=(vect&& f) noexcept
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
        return _new_copy(m, maxs);
    }

    T& operator[](unsigned long index)
    {
        if(index >= size)
        {
            if(maxs == 0)
                this->resizeTo(index + 1);
            else
            {
                unsigned k = 0;
                while(index >= size << ++k);
                this->resizeUp(k);
            }
        }
        smax_(maxs, index + 1);
        return m[index];
    }

    const T& operator[](unsigned long index) const
    {
        ASSERT(index < size, "Vector: index %lu out of range(size: %lu)", index, size);
        DEBUGIFMSG(index >= maxs, "Vector: value at index %lu not set", index);
        return m[index];
    }

    void resizeTo(unsigned long k)
    {
        DEBUGLVLIFMSG(3, k < maxs, "new size smaller than index of last element, "
                                   "some elements will be deleted!");
        _resize(m, size, k);
        size = k;
    }

    void resizeUp(unsigned k = 1)
    {
        unsigned old_size = size;
        m = _resize(m, old_size, size = (old_size + (old_size == 0)) * (1 << k));
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

    iterator begin()
    {
        return new iterator(m);
    }

    iterator at(unsigned long index)
    {
        ASSERT(index <= maxs);
        return new iterator(m + index);
    }

    iterator end()
    {
        return new iterator(m + maxs);
    }

    void swap(unsigned long first, unsigned long second)
    {
        ASSERT((first < size) && (second < size), "Vector: attempt to swap values "
                "out of range(size: %lu, first index: "
                "%lu, second index: %lu)", size, first, second);
        std::swap(m + first, m + second);
    }

    T& max()
    {
        ASSERT(maxs > 0);
        return _max(m, maxs);
    }

    T& min()
    {
        ASSERT(maxs > 0);
        return _min(m, maxs);
    }

    unsigned long minIndex()
    {
        ASSERT(maxs > 0);
        return _minInd(m, maxs);
    }

    unsigned long maxIndex()
    {
        ASSERT(maxs > 0);
        return _maxInd(m, maxs);
    }

    template<typename T2>
    T2& sum(unsigned long from = 0, unsigned long to = vect::last)
    {
        if(to == vect::last)
            to = maxs;
        ASSERT(from <= to);
        ASSERT(to <= size);
        DEBUGLVLIFMSG(3, to > maxs, "Vector: value after index %lu not set, but trying "
                                     "to get sum up to %lu", maxs, to);
        return _sum<T, T2>(m + from, m + to);
    }

    template<bool (*compare)(const T&, const T&) = _less<T>>
    bool checksorted()
    {
        return _checksorted<T, compare>(m, m + maxs);
    }

    template<bool (*compare)(const T&, const T&) = _less<T>>
    bool checksorted(unsigned long from = 0, unsigned long to =
        vect::last)
    {
        ASSERT(from <= to);
        if(to == vect::last)
            to = maxs;
        ASSERT(to <= maxs, "Vector: value after index %lu not set, but trying "
                           "to check if is sorted up to %lu", maxs, to);
        return _checksorted<T, compare>(m + from, m + to);
    }

    template<void (*show)(const T&) = &_tshow>
    void display(unsigned long from = 0, unsigned long to =
        vect::last, const char* del = ", ")
    {
        ASSERT(from <= to);
        if(to == vect::last)
            to = maxs;
        ASSERT(to <= maxs, "Vector: value after index %lu not set, but trying "
                           "to print elements up to %lu", maxs, to);
        if(maxs == 0)
            puts("Vector is empty");
        else
        {
            _tdisplay<T, show>(m + from, to - from, del);
        }
    }

    static const unsigned long last = std::numeric_limits<unsigned long>::max();
};

//TODO
template<typename T>
struct pvect
{};
