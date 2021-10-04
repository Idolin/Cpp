#pragma once

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include "../debug/def_debug.h"
#include "../template/commonmethods.hpp"
#include "../template/type_tags.hpp"
#include "../template/typemethods.hpp"

namespace
{
    template<typename T, typename Enable = typename std::enable_if_t<is_dereferencable<T>::value>>
    inline auto _get(T p) -> typename def_get_by<std::remove_reference_t<decltype(*p)>>::type
    {
        return *p;
    }

    template<typename T, typename Enable = typename std::enable_if_t<!is_dereferencable<T>::value>>
    inline bool _get(T)
    {
        ASSERT(false, "No default method to get value from type %s", typeid(T).name());
        throw std::runtime_error("binary_search: No default method to get value from given type");
        return false;
    }

    template<typename T, typename Enable = void>
    struct _binary_search_base;

    template<typename T>
    struct _binary_search_base<T, typename std::enable_if<std::is_pointer<T>::value>::type>
    {
        T left_border, right_border, cur;

        _binary_search_base(T left, T right, T current): left_border(left),
                                                right_border(right), cur(current)
        {}

        _binary_search_base(T left, T right): left_border(left), right_border(right),
                                               cur(middle())
        {}

        T middle() const
        {
            return left_border + (right_border - left_border) / 2;
        }

        static int epsilon()
        {
            return 1;
        }

        void end_search()
        {
            left_border = reinterpret_cast<T>(1);
            right_border = reinterpret_cast<T>(0);
        }
    };

    template<typename T>
    struct _binary_search_base<T, typename std::enable_if<std::is_integral<T>::value &&
                                                           std::is_unsigned<T>::value>::type>
    {
        T left_border, right_border, cur;

        _binary_search_base(T left, T right, T current): left_border(left),
                                                right_border(right), cur(current)
        {}

        _binary_search_base(T left, T right): left_border(left), right_border(right),
                                                cur(middle())
        {}

        T middle() const
        {
            return left_border + (right_border - left_border) / 2;
        }

        static T epsilon()
        {
            return 1;
        }

        void end_search()
        {
            left_border = 1;
            right_border = 0;
        }
    };

    template<typename T>
    struct _binary_search_base<T, typename std::enable_if<std::is_integral<T>::value &&
                                                           std::is_signed<T>::value>::type>
    {
        T left_border, right_border, cur;

        _binary_search_base(T left, T right, T current): left_border(left),
                                                right_border(right), cur(current)
        {}

        _binary_search_base(T left, T right): left_border (left), right_border(right),
                                               cur(middle())
        {}

    protected:
        T middle() const
        {
            return left_border / 2 + right_border / 2 + (left_border & right_border & 0b1);
        }

        static T epsilon()
        {
            return 1;
        }

        void end_search()
        {
            left_border = 1;
            right_border = 0;
        }
    };

    template<typename T>
    struct _binary_search_base<T, typename std::enable_if<
            std::is_floating_point<T>::value>::type>
    {
        T left_border, right_border, cur;

        _binary_search_base(T left, T right, T current): left_border(left),
                                                right_border(right), cur(current)
        {}

        _binary_search_base(T left, T right): left_border (left), right_border(right),
                                               cur(middle())
        {}

    protected:
        T middle() const
        {
            return left_border / 2 + right_border / 2;
        }

        static T epsilon()
        {
            return std::numeric_limits<T>::epsilon();
        }

        void end_search()
        {
            left_border = 1;
            right_border = 0;
        }
    };
}

template<typename T>
struct binary_search: protected _binary_search_base<T>
{
    // include_last - if true than search in range [left, right], false - [left, right)
    binary_search(T left, T right, bool include_last=false):
        _binary_search_base<T>(left, include_last ? right : right - _binary_search_base<T>::epsilon())
    {
        if(!include_last && left >= right)
            this->end_search();
    }

    binary_search(const binary_search<T> &otr):
        _binary_search_base<T>(otr.left_border, otr.right_border, otr.cur)
    {}

    binary_search<T>& operator=(const binary_search<T> &otr)
    {
        this->left_border = otr.left_border; // not much sense in checking for equality as
        this->cur = otr.cur; // all fields are fast to assign
        this->right_border = otr.right_border;
        return *this;
    }

    bool operator==(const binary_search<T> &otr) const
    {
        return (this->cur == otr.cur) && (this->left_border == otr.left_border) &&
            (this->right_border == otr.right_border);
    }

    bool operator!=(const binary_search<T> &otr) const
    {
        return !(*this == otr);
    }

    T current()
    {
        return this->cur;
    }

    const T current() const
    {
        return this->cur;
    }

    // go to left half
    void left()
    {
        if(this->left_border < this->cur)
            _left();
        else
            this->end_search();
    }

    // go to right half
    void right()
    {
        if(this->cur < this->right_border)
            _right();
        else
            this->end_search();
    }

    // binary search finished
    bool done() const
    {
        return this->left_border > this->right_border;
    }

    // binary search not finished
    operator bool() const
    {
        return !done();
    }

    // if get_last_not -> get last element where condition not true(or previous to first)
    // first iterator: *it > val or next to last, done() returns false if position found and true otherwise
    template<typename R = decltype(_get<T>(std::declval<T>())),
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    T upper_bound(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        if(done())
            return this->cur;
        const T last = (get_last_not) ? this->left_border : this->right_border;
        while(true)
            if(compare(val, get(this->cur)))
            {
                if(this->left_border < this->cur)
                    _left();
                else
                {
                    if(get_last_not)
                    {
                        this->cur -= this->epsilon();
                        if(last == this->left_border)
                            this->end_search();
                    }
                    return this->cur;
                }
            }
            else
            {
                if(this->cur < this->right_border)
                    _right();
                else
                {
                    if(!get_last_not)
                    {
                        this->cur += this->epsilon();
                        if(last == this->right_border)
                            this->end_search();
                    }
                    return this->cur;
                }
            }
    }

    // lower_bound, but also returns not done() with value as std::pair
    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    std::pair<T, bool> upper_bound_check(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        upper_bound<R, compare, get>(val, get_last_not);
        return std::make_pair(this->cur, !done());
    }

    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    size_t upper_bound_pos(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        const T left = this->left_border;
        return upper_bound<R, compare, get>(val, get_last_not) - left;
    }

    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    std::pair<size_t, bool> upper_bound_pos_check(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        size_t pos = upper_bound_pos<R, compare, get>(val, get_last_not);
        return std::make_pair(pos, !done());
    }

    // first iterator: *it >= val or next to last, done() returns false if position found and true otherwise
    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    T lower_bound(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        return upper_bound<R, _not_comp<R, _swap_comp<R, compare>>, get>(val, get_last_not);
    }

    // upper_bound, but also returns not done() with value as std::pair
    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    std::pair<T, bool> lower_bound_check(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        lower_bound<R, compare, get>(val, get_last_not);
        return std::make_pair(this->cur, !done());
    }

    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    size_t lower_bound_pos(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        const T left = this->left_border;
        return lower_bound<R, compare, get>(val, get_last_not) - left;
    }

    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    std::pair<size_t, bool> lower_bound_pos_check(typename def_get_by<R>::type val, bool get_last_not = false)
    {
        size_t pos = lower_bound_pos<R, compare, get>(val, get_last_not);
        return std::make_pair(pos, !done());
    }

    // returns first and last iterator values of which equal to val or [l, r] such thar l > r if not found,
    // if found done() = false, otherwise done() is true
    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    std::pair<T, T> bounds(typename def_get_by<R>::type val)
    {
        if(done())
            return std::make_pair(this->left_border, this->right_border);
        const T first = this->left_border;
        T right = upper_bound<R, compare, get>(val, true);
        if(done())
            return std::make_pair(this->left_border, this->right_border);
        this->left_border = first;
        T left = lower_bound<R, compare, get>(val);
        if(done() || left > right)
        {
            this->end_search();
            return std::make_pair(this->left_border, this->right_border);
        }
        return std::make_pair(left, right);
    }

    // bounds pos or [l, r] such that l > r if no val found
    template<typename R = typename std::remove_pointer<T>::type,
        typename compare_func<R>::type compare = _less<R>, R(*get)(T) = _get<T>>
    std::pair<size_t, size_t> bounds_pos(typename def_get_by<R>::type val)
    {
        const T left = this->left_border;
        std::pair<T, T> range = bounds<R, compare, get>(val);
        if(!done())
            return std::make_pair(range.first - left, range.second - left);
        return std::make_pair(1, 0);
    }

    static const size_t not_found = -1;

private:
    void _left()
    {
        this->right_border = this->cur - this->epsilon();
        this->cur = this->middle();
    }

    void _right()
    {
        this->left_border = this->cur + this->epsilon();
        this->cur = this->middle();
    }
};
