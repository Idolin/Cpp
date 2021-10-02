#pragma once

#include "./forward_iterator.hpp"
#include "./iterator_impl.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iterator_impl_def {

    template<class Impl,
             typename value_type_t,
             typename reference_t,
             typename pointer_t,
             typename difference_t,
             typename iterator_t>
    struct _bidirectional_iterator_adapter:
            // bidirectional iterator is a forward iterator
            _forward_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>,
            /*
             * adds pre-decrement operator--() if not declared based on either
             *  - decrement() method
             *  - getIndex() and setIndex(integral_type) methods
             */
            _preDecrementDefault<Impl, iterator_t>,
            /*
             * adds post-decrement operator--(int) if not declared based on either
             *  - pre-decrement operator--()
             *  - decrement() method
             *  - getIndex() and setIndex(integral_type) methods
             */
            _postDecrementDefault<Impl, iterator_t, reference_t>

    {
        typedef std::bidirectional_iterator_tag iterator_category;

        // forward constructor
        template<typename... Types>
        _bidirectional_iterator_adapter(Types&&... values):
            _forward_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>(std::forward<Types>(values)...)
        {}

        // Use using-declaration for intoducing operator--() and operator--(int) in this class to avoid abmigious name lookup for this operators
        using _preDecrementDefault<Impl, iterator_t>::operator--;
        using _postDecrementDefault<Impl, iterator_t, reference_t>::operator--;
    };

}

template<class Impl, typename value_type_t = typename iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = typename iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = typename iterator_impl_def::get_def_pointer_t<Impl>,
         typename difference_t = std::ptrdiff_t>
struct bidirectional_iterator: iterator_impl_def::_bidirectional_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
        bidirectional_iterator<Impl, value_type_t, reference_t, pointer_t, difference_t>>
{
    // forward constructor
    template<typename... Types>
    bidirectional_iterator(Types&&... values):
        iterator_impl_def::_bidirectional_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
            bidirectional_iterator<Impl, value_type_t, reference_t, pointer_t, difference_t>>(std::forward<Types>(values)...)
    {}
};
