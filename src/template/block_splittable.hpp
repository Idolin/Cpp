//
// Created by cynder on 21.06.19.
//

#pragma once

#include "../debug/def_debug.h"
#include "type_tags.hpp"
#include "valuemethods.hpp"

#include <iterator>
#include <type_traits>

namespace block_splittable
{
    template<typename T, typename Enable = void>
    struct info;

    template<typename T, typename Enable = void>
    struct __info_itr;

    template<typename T>
    struct __info_itr<T, typename std::enable_if_t<is_block_iterable_class<T>::value>>
    {
        typedef decltype(*(std::declval<
                typename std::add_lvalue_reference<
                        typename std::add_const<T>::type>::type>().cbegin())) _RType;

        typedef typename info<_RType>::RType RType;

        enum
        {
            const_itr_size = std::is_integral<_RType>::value || std::is_pointer<_RType>::value
        };
    };

    template<typename T>
    struct __info_itr<T, typename std::enable_if_t<!is_block_iterable_class<T>::value>>
    {
        typedef decltype(std::declval<
                typename std::add_lvalue_reference<
                        typename std::add_const<T>::type>::type>().operator[][static_cast<size_t>(0)]) _RType;

        typedef typename info<_RType>::RType RType;

        enum
        {
            const_itr_size = std::is_integral<_RType>::value || std::is_pointer<_RType>::value
        };
    };

    template<typename T>
    struct info<T, typename std::enable_if_t<std::is_integral<T>::value || std::is_pointer<T>::value>>
    {
        typedef const typename _valueMethods<T>::to_unsigned_type RType;
        typedef typename _valueMethods<T>::to_unsigned_type RAType;

        enum
        {
            const_get_size = true,
            const_itr_size = true
        };
    };


    template<typename T>
    struct info<T, typename std::enable_if_t<std::is_class<T>::value &&
                                                              is_block_splittable<T>::value>>: __info_itr<T>
    {
        typedef decltype(std::declval<
                typename std::add_lvalue_reference<
                        typename std::add_const<T>::type>::type>().operator[][static_cast<size_t>(0)]) _RAType;

        typedef typename info<_RType>::RAType RAType;

        enum
        {
            const_get_size = std::is_integral<_RAType>::value || std::is_pointer<_RAType>::value
        };
    };

    template<typename T>
    inline typename std::enable_if_t<std::is_integral<T>::value || std::is_pointer<T>::value, size_t>
    get_blocks_amount(const T& o)
    {
        return 1;
    }

    template<typename T>
    inline typename std::enable_if_t<std::is_integral<T>::value || std::is_pointer<T>::value,
            typename info<T>::RAType>
    get_block(size_t i, const T& o)
    {
        ASSERT(i == 0);

        return _valueMethods<T>::to_unsigned(o);
    }

    template<typename T>
    inline typename std::enable_if_t<std::is_class<T>::value && is_block_splittable<T>::value, size_t>
    get_blocks_amount(const T& o)
    {
        if(info<T>::const_get_size)
            return o.size();

        size_t blocks_amount = 0;

        for(size_t i = 0;i < o.size();i++)
            blocks_amount += get_blocks_amount(o[i]);

        return blocks_amount;
    }

    template<typename T>
    inline typename std::enable_if_t<std::is_class<T>::value && is_block_splittable<T>::value,
            typename info<T>::RAType>
    get_block(size_t i, const T& o)
    {
        ASSERT(i < get_blocks_amount(o));

        if(info<T>::const_get_size)
            return o[i];

        for(size_t j = 0;j < o.size();j++)
        {
            size_t current_blocks_amount = get_blocks_amount(o[j]);
            if(i < current_blocks_amount)
                return get_block(i, o[j]);
            i -= current_blocks_amount;
        }
    }

    template<typename T, typename Enable = void>
    struct iterator;

    template<typename T>
    struct iterator<T, typename std::enable_if_t<itr<T>::const_get_type>>:
            public std::iterator<std::bidirectional_iterator_tag, typename itr<T>::RAType>
    {
    private:
        const T *ptr;
        size_t i, size;

        iterator(const T *ptr, size_t i, size_t size): ptr(ptr), i(i), size(size)
        {}

        static const size_t is_end = std::numeric_limits<size_t>::max();

    public:
        iterator(): ptr(nullptr), i(is_end), size(0)
        {}

        iterator(const T &ptr): ptr(&ptr), i(0), size(get_blocks_amount(ptr))
        {}

        iterator(const iterator& otr): ptr(otr.ptr), i(otr.i), size(otr.size)
        {}

        iterator& operator=(iterator const &otr)
        {
            ptr = otr.ptr;
            i = otr.i;
            size = otr.size;
            return *this;
        }

        bool operator==(const iterator &otr) const
        {
            if((i == is_end) ^ (otr.i == is_end))
                return false;
            return (i == otr.i) && ((ptr == otr.ptr) || (i == is_end));
        }

        bool operator!=(const iterator &otr) const
        {
            return !(*this == otr);
        }

        iterator& operator++()
        {
            if(++i >= size)
                i = is_end;
            return *this;
        }

        iterator operator++(int)
        {
            iterator copy = *this;
            ++*this;
            return copy;
        }

        iterator &operator--()
        {
            if(i == is_end)
                i = size - 1;
            else
                i--;
            return *this;
        }

        iterator operator--(int)
        {
            iterator copy = *this;
            --*this;
            return copy;
        }

        typename itr<T>::RAType& operator*() const
        {
            return (*ptr)[i];
        }

        typename itr<T>::RAType* operator->() const
        {
            return &((*ptr)[i]);
        }
    };

    template<typename T>
    struct iterator<T, typename std::enable_if_t<!itr<T>::const_get_type>>:
            public std::iterator<std::bidirectional_iterator_tag, typename itr<T>::RAType>
    {
    private:
        typedef iterator<typename itr<T>::_RAType> itr;

        const T *ptr;
        itr it;
        size_t i, size;

        iterator(const T *ptr, itr it, size_t i, size_t size): ptr(ptr), it(it), i(i), size(size)
        {}

        static const size_t is_end = std::numeric_limits<size_t>::max();

    public:
        iterator(): ptr(nullptr), it(), i(is_end), size(0)
        {}

        iterator(const T &ptr): ptr(&ptr), it(), i(0), size(get_blocks_amount(ptr))
        {
            if(size)
                it = itr(ptr[0]);
        }

        iterator(const iterator& otr): ptr(otr.ptr), it(otr.it), i(otr.i), size(otr.size)
        {}

        iterator& operator=(iterator const &otr)
        {
            ptr = otr.ptr;
            it = otr.it;
            i = otr.i;
            size = otr.size;
            return *this;
        }

        bool operator==(const iterator &otr) const
        {
            if((i == is_end) ^ (otr.i == is_end))
                return false;
            return (i == otr.i) && ((ptr == otr.ptr) && (it == otr.it) || (i == is_end));
        }

        bool operator!=(const iterator &otr) const
        {
            return !(*this == otr);
        }

        iterator& operator++()
        {
            if(++it == itr())
            {
                if(++i == size)
                    i = is_end;
                else
                    it = itr((*ptr)[i]);
            }
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
            if(i == is_end)
                i = size - 1;
            else if(--it == itr())
            {
                if(--i != is_end)
                    it = --(--itr((*ptr)[i]));
                else
                    it = itr();
            }
            return *this;
        }

        iterator operator--(int)
        {
            iterator copy = *this;
            --*this;
            return copy;
        }

        typename itr<T>::RAType& operator*() const
        {
            return *it;
        }

        typename itr<T>::RAType* operator->() const
        {
            return &(*it);
        }
    };

    template<typename T>
    struct itr<T, typename std::enable_if_t<std::is_integral<T>::value || std::is_pointer<T>::value>>
    {
    private:
        struct iterator: public std::const_iterator<std::bidirectional_iterator_tag, const T>
        {
        private:
            T val;
            bool is_end;

            iterator(const T val, bool is_end): val(val), is_end(is_end)
            {}

        public:
            iterator(): is_end(true)
            {}

            iterator(const T val): val(val), is_end(false)
            {}

            iterator(const iterator& otr): val(otr.val), is_end(otr.is_end)
            {}

            iterator& operator=(iterator const &otr)
            {
                val = otr.val;
                is_end = otr.is_end;
                return *this;
            }

            bool operator==(const iterator &otr) const
            {
                return (is_end == otr.is_end) && (is_end || val == otr.val);
            }

            bool operator!=(const iterator &otr) const
            {
                return !(*this == otr);
            }

            iterator& operator++()
            {
                is_end = true;
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
                is_end = false;
                return *this;
            }

            iterator operator--(int)
            {
                iterator copy = *this;
                --*this;
                return copy;
            }

            typename info<T>::RType& operator*() const
            {
                return val;
            }

            typename info<T>::RType* operator->() const
            {
                return &val;
            }
        };

    public:
        T val;

        itr(const T val): val(val)
        {}

        iterator cbegin() const
        {
            return iterator(val);
        }

        iterator cend() const
        {
            return iterator(val, true);
        }
    };

    itr<...>
}

template<typename T, typename R, typename = typename std::enable_if_t<is_block_splittable<T>::value>,
        typename = typename std::enable_if_t<std::is_unsigned<R>::value && !std::is_same<R, bool>::value>>
struct block_splittable_adapter
{
    const T *obj;

    block_splittable_adapter(const T &obj): obj(&obj)
    {}

    ~block_splittable_adapter() = default;

    R operator[](size_t i) const
    {
        ASSERT(i < size());


    }
};
