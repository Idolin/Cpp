#include <cassert>
#include <string>
#include <iostream>
#include "../other/arraymethods.hxx"

using std::string;
using std::cout;
using std::endl;

struct mdeque
{
    size_t amount, dsize, start, end;
    string *a;

    mdeque() : amount(0), dsize(2), start(0), end(0), a(new string[dsize])
    {
        for(unsigned i = 0; i < dsize; i++)
            a[i] = "0";
    }

    mdeque(const mdeque &d) : amount(0), dsize(d.dsize), start(d.start), end(d.end), a(new string[dsize])
    {
        _copy(a, dsize, d.a);
    }

    ~mdeque()
    {
        delete[] a;
    }

    mdeque &operator=(const mdeque &d)
    {
        if(this != &d)
        {
            this->dsize = d.dsize;
            this->start = d.start;
            this->end = d.end;
            this->amount = d.amount;
            delete[] a;
            a = new string[dsize];
            _copy(a, dsize, d.a);
        }
        return *this;
    }

    bool empty() const
    {
        return (amount == 0);
    }

    void clear()
    {
        if(dsize > 10)
        {
            delete[] a;
            dsize = 0;
        }
        start = 0;
        end = 1;
        amount = 0;
    }

    void resize()
    {
        size_t new_size = dsize * 2 + 7;
        string *n = new string[new_size];
        _copy(n, amount, to_array());
        start = 0;
        end = amount;
        delete[] a;
        a = n;
        dsize = new_size;
    }

    string *to_array() const
    {
        string *array = new string[amount];
        if(!amount)
            return array;
        if(start >= end)
        {
            _copy(array, dsize - start, a + start);
            _copy(array + start, end, a);
        } else
            _copy(array, end - start, a);
        return array;
    }

    string &back()
    {
        if(!amount)
            throw 5;
        if(end == 0)
            return a[dsize - 1];
        return a[end - 1];
    }

    const string &back() const
    {
        if(!amount)
            throw 5;
        if(end == 0)
            return a[dsize - 1];
        return a[end - 1];
    }

    string &front()
    {
        if(!amount)
            throw 5;
        if(start == dsize)
            return a[0];
        return a[start];
    }

    const string &front() const
    {
        if(!amount)
            throw 5;
        if(start == dsize)
            return a[0];
        return a[start];
    }

    string pop_back()
    {
        if(!amount--)
            throw 5;
        if(end-- == 0)
            end = dsize;
        string r = a[end];
        a[end].~basic_string();
        return r;
    }

    void push_back(const string &s)
    {
        if(start == end)
            resize();
        a[end++] = string(s);
        if(end == dsize)
            end = 0;
        amount++;
    }

    string pop_front()
    {
        if(!amount--)
            throw 5;
        if(++start == dsize)
            start = 0;
        string r = a[start];
        a[start].erase();
        return r;
    }

    void push_front(const string &s)
    {
        if(++amount == dsize)
            resize();
        if(start-- == 0)
            start = dsize - 1;
        a[start] = s;
    }

    size_t size() const
    {
        return amount;
    }

    string &operator[](size_t index)
    {
        if(index >= amount)
            throw 5;
        size_t p = start + index;
        if(p >= dsize)
            p -= dsize;
        return a[p];
    }

    const string &operator[](size_t index) const
    {
        if(index >= amount)
            throw 5;
        size_t p = start + index;
        if(p >= dsize)
            p -= dsize;
        return a[p];
    }
};
