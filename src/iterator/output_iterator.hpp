#pragma once

#include "./iterator_impl.hpp"
#include "../template/type_tags.hpp"

#include <iterator>
#include <type_traits>

template<class Impl>
struct output_iterator: Impl,
        //adds pre-increment operator++() if not declared based on increment() method
        iterator_impl_def::_preIncrementDefault<Impl, output_iterator<Impl>>,
        /*
         * adds post-increment operator++(int) if not declared based on either
         *  - pre-increment operator++()
         *  - increment() method
         */
        iterator_impl_def::_postIncrementDefault<Impl, output_iterator<Impl>>
{
    typedef std::output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;

    static_assert(is_dereferencable_v<Impl>, "Output iterator must be dereferencable");

    // forward constructor
    template<typename... Types>
    output_iterator(Types&&... values): Impl(std::forward<Types>(values)...)
    {}

    /*
     * Increment operators implemented here, in upper score,
     *  to ensure they will be called instead of original ones declared in Impl class
     */
    output_iterator<Impl>& operator++()
    {
        iterator_impl_def::_preIncrementDefault<Impl, output_iterator<Impl>>::_preIncrement();
        return *static_cast<output_iterator<Impl>*>(this);
    }

    auto operator++(int)
    {
        typedef decltype(iterator_impl_def::_postIncrementDefault<Impl, output_iterator<Impl>>::_postIncrement()) return_t;
        static_assert(std::is_convertible<return_t, const Impl&>::value, "Output iterator post-increment return type must be convertible to const It&");

        return iterator_impl_def::_postIncrementDefault<Impl, output_iterator<Impl>>::_postIncrement();
    }
};
