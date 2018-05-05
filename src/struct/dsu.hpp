#pragma once

#include "../other/rand.h"
#include "../container/vector.hpp"
#include "../template/struct_tags.hpp"
#include "../template/t_useful.hpp"
#include "../template/valuemethods.hpp"

#include <algorithm>

using std::min;
using std::max;
using std::pair;
using std::make_pair;

namespace internal
{
    template<bool balanced>
    struct element_balance
    {};

    template<>
    struct element_balance<true>
    {
        unsigned char balance;

        element_balance(): balance(0)
        {}
    };

    template<bool fast_range_update>
    struct element_range
    {
        element_range(unsigned i = 0)
        {}
    };

    template<>
    struct element_range<true>
    {
        unsigned right_border;

        element_range(unsigned i = 0): right_border(i)
        {}
    };

    template<bool balanced, bool fast_range_update>
    struct element_base: element_balance<balanced>, element_range<fast_range_update>
    {
        unsigned pointer;

        element_base()
        {}

        element_base(unsigned index): element_balance<balanced>(),
                                       element_range<fast_range_update>(index),
                                       pointer(index)
        {}

        void merge(element_base& otr)
        {
            pointer = otr.pointer;
        }
    };
}

template<typename TData = void, bool balanced = false,
        bool fast_range_update = false, typename = typename std::enable_if<
                std::is_same<TData, void>::value ||
                is_mergeable<TData>::value>::type>
struct dsu
{
private:
    struct element: internal::element_base<balanced, fast_range_update>
    {
        TData data;

        element()
        {}

        element(unsigned index, TData& data):
                internal::element_base<balanced, fast_range_update>(index), data(data)
        {}

        void merge(element&& otr)
        {
            data.merge(otr.data);
            internal::element_base<balanced, fast_range_update>::merge(otr);
        }
    };

    unsigned setsAmount;
    vect<element> s;

public:
    dsu(): setsAmount(0), s()
    {}

    template<TData def = TData()>
    dsu(unsigned count): setsAmount(count), s(count)
    {
        this->add<def>(count);
    }

    dsu(TData *array, unsigned len): setsAmount(len), s(len)
    {
        this->add(array, len);
    }

    ~dsu()
    {}

    template<TData def = TData()>
    void add(unsigned count = 1)
    {
        this->setsAmount += count;
        this->s.capacityEnsure(this->s.size() + count);
        for(unsigned i = this->s.size();count > 0;i++,count--)
            this->s[i] = element(i, def);
    }

    void add(TData *array, unsigned len)
    {
        this->setsAmount += len;
        this->s.capacityEnsure(this->s.size() + len);
        for(unsigned i = this->s.size();len > 0;i++,len--)
            this->s[i] = element(i, array[i]);
    }

    unsigned size()
    {
        return setsAmount;
    }

    unsigned getSetID(unsigned p)
    {
        if(p < s.size())
        {
            unsigned t = s[p].pointer;
            if(s[t].pointer != t)
            {
                s[p].pointer = this->getSetID(t);
                s[p].merge(s[s[p].pointer]);
            }
            return s[p].pointer;
        }
        else
        {
            add(p - s.size() + 1);
            return p;
        }
    }

    TData& getElement(unsigned setID)
    {
        return this->s[setID].data;
    }

    void unite(unsigned first, unsigned second)
    {
        unsigned firstSetID = this->getSetID(first);
        unsigned secondSetID = this->getSetID(second);
        if(firstSetID != secondSetID)
            uniteImpl<_rank<_valueMethods<bool>::to_unsigned(
                    balanced)>::value>(firstSetID, secondSetID);
    }

    template<_rank<1>::valueType ignored>
    void uniteImpl(unsigned firstID, unsigned secondID)
    {
        if(s[firstID].balance >= s[secondID].balance)
        {
            if(s[firstID].balance == s[secondID].balance)
                s[firstID].balance++;
            s[secondID].pointer = firstID;
            s[secondID].merge(s[firstID]);
        }
        else
        {
            s[firstID].pointer = secondID;
            s[firstID].merge(s[secondID]);
        }
    }

    template<_rank<0>::valueType ignored>
    void uniteImpl(unsigned firstID, unsigned secondID)
    {
        if(randomB())
        {
            s[secondID].pointer = firstID;
            s[secondID].merge(s[firstID]);
        }
        else
        {
            s[firstID].pointer = secondID;
            s[firstID].merge(s[secondID]);
        }
    }

    void uniteRange(unsigned from, unsigned to)
    {
        ASSERT(from <= to);
        if(to > s.size())
            this->add(to - s.size());
        uniteRangeImpl<_rank<_valueMethods<bool>::to_unsigned(
                fast_range_update)>::value>(from, to);
    }

    template<_rank<1>::valueType ignored>
    void uniteRangeImpl(unsigned from, unsigned to)
    {
        to--;
        unsigned right_border = s[to].right_border;
        while(s[from].right_border != right_border)
        {
            unite(from, to);
            unsigned next = s[from].right_border + 1;
            s[from].right_border = right_border;
            from = next;
        }
    }

    template<_rank<0>::valueType ignored>
    void uniteRangeImpl(unsigned from, unsigned to)
    {
        for(;from < to - 1;from++)
            this->unite(from, from + 1);
    }

    bool sameSet(unsigned first, unsigned second)
    {
        return getSetID(first) == getSetID(second);
    }
};

template<bool balanced,
        bool fast_range_update>
struct dsu<void, balanced, fast_range_update>
{
private:
    using element = internal::element_base<balanced, fast_range_update>;

    unsigned setsAmount;
    vect<element> s;

public:
    dsu(unsigned count = 0): setsAmount(count), s(count)
    {
        this->add(count);
    }

    ~dsu()
    {}

    void add(unsigned count = 1)
    {
        this->s.capacityEnsure(this->s.size() + count);
        for(unsigned i = static_cast<unsigned>(this -> s.size());count > 0;i++, count--)
            this->s[i] = element(i);
    }

    unsigned size()
    {
        return setsAmount;
    }

    unsigned getSetID(unsigned p)
    {
        if(p < s.size())
        {
            unsigned t = s[p].pointer;
            if(s[t].pointer != t)
            {
                s[p].pointer = this->getSetID(t);
                s[p].merge(s[s[p].pointer]);
            }
            return s[p].pointer;
        }
        else
        {
            this -> add(p - s.size() + 1);
            return p;
        }
    }

    void unite(unsigned first, unsigned second)
    {
        unsigned firstSetID = this->getSetID(first);
        unsigned secondSetID = this->getSetID(second);
        if(firstSetID != secondSetID)
            uniteImpl<_rank<_valueMethods<bool>::to_unsigned(
                    balanced)>::value>(firstSetID, secondSetID);
    }

    template<_rank<1>::valueType ignored>
    void uniteImpl(unsigned firstID, unsigned secondID)
    {
        if(s[firstID].balance >= s[secondID].balance)
        {
            if(s[firstID].balance == s[secondID].balance)
                s[firstID].balance++;
            s[secondID].pointer = firstID;
            s[secondID].merge(s[firstID]);
        }
        else
        {
            s[firstID].pointer = secondID;
            s[firstID].merge(s[secondID]);
        }
    }

    template<_rank<0>::valueType ignored>
    void uniteImpl(unsigned firstID, unsigned secondID)
    {
        if(randomB())
        {
            s[secondID].pointer = firstID;
            s[secondID].merge(s[firstID]);
        }
        else
        {
            s[firstID].pointer = secondID;
            s[firstID].merge(s[secondID]);
        }
    }

    void uniteRange(unsigned from, unsigned to)
    {
        ASSERT(from <= to);
        if(to > s.size())
            this->add(to - s.size());
        uniteRangeImpl<_rank<_valueMethods<bool>::to_unsigned(
                fast_range_update)>::value>(from, to);
    }

    template<_rank<1>::valueType ignored>
    void uniteRangeImpl(unsigned from, unsigned to)
    {
        to--;
        unsigned right_border = s[to].right_border;
        while(s[from].right_border != right_border)
        {
            unite(from, to);
            unsigned next = s[from].right_border + 1;
            s[from].right_border = right_border;
            from = next;
        }
    }

    template<_rank<0>::valueType ignored>
    void uniteRangeImpl(unsigned from, unsigned to)
    {
        for(;from < to - 1;from++)
            this->unite(from, from + 1);
    }

    bool sameSet(unsigned first, unsigned second)
    {
        return getSetID(first) == getSetID(second);
    }
};