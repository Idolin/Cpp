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
    unsigned long v_size, max_used;

public:
    vect(unsigned size = 4): m(new T[size]), v_size(size), max_used(0)
    {}

    vect(unsigned size, T def_value): m(new T[size]), v_size(size), max_used(size)
    {
        _fill(m, size, def_value);
    }

    vect(vect const &f): m(new T[f.v_size]), v_size(f.v_size), max_used(f.max_used)
    {
        _copy(m, v_size, f.m);
    }

    vect(vect&& f): m(f.m), v_size(f.v_size), max_used(f.max_used)
    {
        f.m = nullptr;
        f.v_size = 0;
        f.max_used = 0;
    }

    vect& operator=(vect const &f)
    {
        if(this != &f)
        {
            delete[] m;
            max_used = f.max_used;
            v_size = f.v_size;
            m = _new_copy(f.m, v_size);
        }
        return *this;
    }

    vect& operator=(vect&& f) noexcept
    {
        if(this != &f)
        {
            delete[] m;
            m = f.m;
            v_size = f.v_size;
            max_used = f.max_used;
            f.m = nullptr;
            f.v_size = 0;
            f.max_used = 0;
        }
        return *this;
    }

    ~vect()
    {
        delete[] m;
    }

    operator T*()
    {
        return _new_copy(m, max_used);
    }

    T *toArray(unsigned long from = 0, unsigned long to = vect::last)
    {
        if(to == vect::last)
            to = max_used;
        return _new_copy(m + from, m + to);
    }

    T& operator[](unsigned long index)
    {
        if(index >= v_size)
        {
            if(v_size == 0)
                this->resizeTo(index + 1);
            else
            {
                unsigned k = 0;
                while(index >= v_size << ++k);
                this->resizeUp(k);
            }
        }
        smax_(max_used, index + 1);
        return m[index];
    }

    const T& operator[](unsigned long index) const
    {
        ASSERT(index < max_used, "Vector: index %lu out of range(max set: %lu)", index, max_used);
        return m[index];
    }

    unsigned long size() const
    {
        return max_used;
    }

    bool empty() const
    {
        return (max_used == 0);
    }

    void resizeTo(unsigned long k)
    {
        DEBUGLVLIFMSG(3, k < max_used, "new size smaller than index of last element, "
                                   "some elements will be deleted!");
        m = _resize(m, max_used, k);
        v_size = k;
        smin_(max_used, v_size);
    }

    void resizeUp(unsigned k = 1)
    {
        v_size = (v_size + (v_size == 0)) * (1 << k);
        m = _resize(m, max_used, v_size);
    }

    void capacityEnsure(unsigned long k)
    {
        if(k > v_size)
            resizeTo(k);
    }

    T& back()
    {
        ASSERT(max_used > 0);
        return m[max_used - 1];
    }

    const T& back() const
    {
        ASSERT(max_used > 0);
        return m[max_used - 1];
    }

    void push(T x)
    {
        if(max_used == v_size)
            this->resizeUp();
        m[max_used++] = x;
    }

    T&& pop()
    {
        ASSERT(max_used > 0);
        return std::move(m[--max_used]);
    }

    iterator begin()
    {
        return iterator(m);
    }

    iterator at(unsigned long index)
    {
        ASSERT(index <= max_used);
        return iterator(m + index);
    }

    iterator end()
    {
        return iterator(m + max_used);
    }

    void swap(unsigned long first, unsigned long second)
    {
        ASSERT((first < max_used) && (second < max_used), "Vector: attempt to swap values "
                "out of range(max set: %lu, first index: "
                "%lu, second index: %lu)", max_used, first, second);
        std::swap(m + first, m + second);
    }

    T& max()
    {
        ASSERT(max_used > 0);
        return _max(m, max_used);
    }

    T& min()
    {
        ASSERT(max_used > 0);
        return _min(m, max_used);
    }

    unsigned long minIndex()
    {
        ASSERT(max_used > 0);
        return _minInd(m, max_used);
    }

    unsigned long maxIndex()
    {
        ASSERT(max_used > 0);
        return _maxInd(m, max_used);
    }

    template<typename T2>
    T2& sum(unsigned long from = 0, unsigned long to = vect::last)
    {
        if(to == vect::last)
            to = max_used;
        ASSERT(from <= to);
        ASSERT(to <= max_used, "Vector: value after index %lu not set, but trying "
                               "to get sum up to %lu", max_used, to);
        return _sum<T, T2>(m + from, m + to);
    }

    template<bool (*compare)(const T&, const T&) = _less<T>>
    bool checksorted()
    {
        return _checksorted<T, compare>(m, m + max_used);
    }

    template<bool (*compare)(const T&, const T&) = _less<T>>
    bool checksorted(unsigned long from = 0, unsigned long to =
        vect::last)
    {
        ASSERT(from <= to);
        if(to == vect::last)
            to = max_used;
        ASSERT(to <= max_used, "Vector: value after index %lu not set, but trying "
                           "to check if is sorted up to %lu", max_used, to);
        return _checksorted<T, compare>(m + from, m + to);
    }

    template<void (*show)(const T&) = &_tshow>
    void display(unsigned long from = 0, unsigned long to =
        vect::last, const char* del = ", ")
    {
        ASSERT(from <= to);
        if(to == vect::last)
            to = max_used;
        ASSERT(to <= max_used, "Vector: value after index %lu not set, but trying "
                           "to print elements up to %lu", max_used, to);
        if(max_used == 0)
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
