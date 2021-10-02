#pragma once

#include "./iterator_impl.hpp"
#include "../template/type_tags.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

template<class Impl>
struct output_iterator: public Impl,
        /*
         * adds pre-increment operator++() if not declared based on either
         *  - increment() method
         *  - getIndex() and setIndex(integral_type) methods
         */
        iterator_impl_def::_preIncrementDefault<Impl, output_iterator<Impl>>,
        /*
         * adds post-increment operator++(int) if not declared based on either
         *  - pre-increment operator++()
         *  - increment() method
         *  - getIndex() and setIndex(integral_type) methods
         */
        iterator_impl_def::_postIncrementDefault<Impl, output_iterator<Impl>, void>
{
private:
    typedef output_iterator<Impl> self;
    typedef decltype(std::declval<iterator_impl_def::_postIncrementDefault<Impl, self, void>>().operator++(0)) post_increment_return_t;

public:
    typedef std::output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void reference;
    typedef void pointer;
    typedef void difference_type;

    static_assert(is_dereferencable_v<Impl>, "Output iterator must be dereferencable");

    static_assert(std::is_convertible<post_increment_return_t, const Impl&>::value,
            "Output iterator post-increment return type must be convertible to const It&");

    // forward constructor
    template<typename... Types>
    output_iterator(Types&&... values): Impl(std::forward<Types>(values)...)
    {}

    // Use using-declaration for intoducing operator++() and operator++(int) in this class to avoid abmigious name lookup for this operators
    using iterator_impl_def::_preIncrementDefault<Impl, self>::operator++;
    using iterator_impl_def::_postIncrementDefault<Impl, self, void>::operator++;
};
