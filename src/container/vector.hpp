#pragma once

#include "../template/arraymethods.hpp"
#include "../other/common.hpp"
#include "../debug/def_debug.h"
#include "../template/displaymethods.hpp"

#include <algorithm>
#include <iterator>
#include <cstdio>
#include <mutex>

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

        iterator(iterator&& otr): ptr(otr.ptr)
        {
            otr.ptr = nullptr;
        }

        iterator& operator=(const iterator& otr)
        {
            ptr = otr.ptr;
            return *this;
        }

        iterator& operator=(iterator&& otr)
        {
            if(this != &otr)
            {
                ptr = otr.ptr;
                otr.ptr = nullptr;
            }
            return *this;
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

        T* operator->()
        {
            return ptr;
        }
    };

protected:
    T *m;
    unsigned long v_size, max_used;
    mutable std::recursive_mutex _lock;

public:
    vect(unsigned size = 4): m(new T[size]), v_size(size), max_used(0)
    {}

    vect(unsigned size, T def_value): m(new T[size]), v_size(size), max_used(size)
    {
        fill(m, size, def_value);
    }

    vect(vect const &f): m(new T[f.v_size]), v_size(f.v_size), max_used(f.max_used)
    {
        f.lock();
        copy_array(f.m, v_size, m);
        f.unlock();
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
            f.lock();
            delete[] m;
            max_used = f.max_used;
            v_size = f.v_size;
            m = new_array_copy(f.m, v_size);
            f.unlock();
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
        _lock.lock();
        T *copy = new_array_copy(m, max_used);
        _lock.unlock();
        return copy;
    }

    T *toArray(unsigned long from = 0, unsigned long to = vect::last)
    {
        _lock.lock();
        if(to == vect::last)
            to = max_used;
        T *copy = new_array_copy(m + from, m + to);
        _lock.unlock();
        return copy;
    }

    T& operator[](unsigned long index)
    {
        _lock.lock();
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
        set_max(max_used, index + 1);
        _lock.unlock();
        return m[index];
    }

    const T& at(unsigned long index) const
    {
        ASSERT(index < max_used, "Vector: index %lu out of range(max set: %lu)", index, max_used);
        return m[index];
    }

    const T& operator[](unsigned long index) const
    {
        return at(index);
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
        _lock.lock();
        m = resize_array(m, max_used, k);
        v_size = k;
        set_min(max_used, v_size);
        _lock.unlock();
    }

    void resizeUp(unsigned k = 1)
    {
        _lock.lock();
        v_size = (v_size + (v_size == 0)) * (1 << k);
        m = resize_array(m, max_used, v_size);
        _lock.unlock();
    }

    void capacityEnsure(unsigned long k)
    {
        if(k > v_size)
            resizeTo(k);
    }

    void clear()
    {
        _lock.lock();
        max_used = 0;
        _lock.unlock();
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

    void push(const T& x)
    {
        _lock.lock();
        if(max_used == v_size)
            this->resizeUp();
        m[max_used++] = x;
        _lock.unlock();
    }

    T&& pop()
    {
        _lock.lock();
        ASSERT(max_used > 0);
        max_used--;
        _lock.unlock();
        return std::move(m[max_used]);
    }

    iterator begin()
    {
        return iterator(m);
    }

    iterator iterator_at(unsigned long index)
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
        _lock.lock();
        ASSERT((first < max_used) && (second < max_used), "Vector: attempt to swap values "
                "out of range(max set: %lu, first index: "
                "%lu, second index: %lu)", max_used, first, second);
        std::swap(m[first], m[second]);
        _lock.unlock();
    }

    T& max()
    {
        _lock.lock();
        ASSERT(max_used > 0);
        T &m = _max(m, max_used);
        _lock.unlock();
        return m;
    }

    T& min()
    {
        _lock.lock();
        ASSERT(max_used > 0);
        T &m = _min(m, max_used);
        _lock.unlock();
        return m;
    }

    unsigned long minIndex()
    {
        _lock.lock();
        ASSERT(max_used > 0);
        unsigned ind = index_of_min(m, max_used);
        _lock.unlock();
        return ind;
    }

    unsigned long maxIndex()
    {
        _lock.lock();
        ASSERT(max_used > 0);
        unsigned ind = index_of_max(m, max_used);
        _lock.unlock();
        return ind;
    }

    template<typename R>
    R sum(unsigned long from = 0, unsigned long to = vect::last)
    {
        _lock.lock();
        if(to == vect::last)
            to = max_used;
        ASSERT(from <= to);
        ASSERT(to <= max_used, "Vector: value after index %lu not set, but trying "
                               "to get sum up to %lu", max_used, to);
        R r = array_sum<T, R>(m + from, m + to);
        _lock.unlock();
        return r;
    }

    template<bool (*compare)(const T&, const T&) = def_less<T>>
    bool checksorted()
    {
        _lock.lock();
        bool sorted = array_checksorted<T, compare>(m, m + max_used);
        _lock.unlock();
        return sorted;
    }

    template<bool (*compare)(const T&, const T&) = def_less<T>>
    bool checksorted(unsigned long from = 0, unsigned long to =
        vect::last)
    {
        ASSERT(from <= to);
        if(to == vect::last)
            to = max_used;
        _lock.lock();
        ASSERT(to <= max_used, "Vector: value after index %lu not set, but trying "
                           "to check if is sorted up to %lu", max_used, to);
        bool sorted = array_checksorted<T, compare>(m + from, m + to);
        _lock.unlock();
        return sorted;
    }

    template<void (*show)(const T&) = &_tshow>
    void display(unsigned long from = 0, unsigned long to =
        vect::last, const char* del = ", ")
    {
        ASSERT(from <= to);
        if(to == vect::last)
            to = max_used;
        _lock.lock();
        ASSERT(to <= max_used, "Vector: value after index %lu not set, but trying "
                           "to print elements up to %lu", max_used, to);
        if(max_used == 0)
            puts("Vector is empty");
        else
        {
            _tdisplay<T, show>(m + from, to - from, del);
        }
        _lock.unlock();
    }

    void lock() const
    {
        _lock.lock();
    }

    bool try_lock() const
    {
        return _lock.try_lock();
    }

    void unlock() const
    {
        _lock.unlock();
    }

    static const unsigned long last = std::numeric_limits<unsigned long>::max();
};

//TODO
template<typename T>
struct pvect
{};
