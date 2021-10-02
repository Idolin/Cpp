#pragma once

#include "./iterator_impl.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iterator_impl_def {

    template<class Impl, typename value_type_t,
             typename reference_t,
             typename pointer_t,
             typename difference_t,
             typename iterator_t>
    struct _input_iterator_adapter: public Impl,
            // adds operator==(const It&) if not declared (and not declared non-member function operator==(const It&, const It&)) based on getIndex() method
            iterator_impl_def::_equalDefault<Impl, iterator_t>,
            /*
             * adds operator!=(const It&) if not declared (and not declared non-member function operator!=(const It&, const It&)) based on either
             *  - operator==(const It&)
             *  - non-member function operator==(const It&, const It&)
             *  - getIndex() method
             */
            iterator_impl_def::_notEqualDefault<Impl, iterator_t>,
            // adds operator-> if not declared based on operator*
            iterator_impl_def::_arrowDefault<Impl, iterator_t, pointer_t>,
            /*
             * adds pre-increment operator++() if not declared based on either
             *  - increment() method
             *  - getIndex() and setIndex(integral_type) methods
             */
            iterator_impl_def::_preIncrementDefault<Impl, iterator_t>,
            /*
             * adds post-increment operator++(int) if not declared based on either
             *  - pre-increment operator++()
             *  - increment() method
             *  - getIndex() and setIndex(integral_type) methods
             */
            iterator_impl_def::_postIncrementDefault<Impl, iterator_t, value_type_t>
    {
        typedef std::input_iterator_tag iterator_category;
        typedef value_type_t value_type;
        typedef reference_t reference;
        typedef pointer_t pointer;
        typedef difference_t difference_type;

        static_assert(std::is_integral<difference_type>::value,
            "Iterator difference_type better to be integral in order to provide type for algortihms like std::distance");
        static_assert(std::is_signed<difference_type>::value,
            "Iterator difference_type must be signed to represent negative offset between iterators");
        static_assert(std::is_convertible<reference, value_type>::value,
            "Input iterator reference type needs to be convertible to value_type");
        static_assert(std::is_same<decltype(std::declval<Impl>().operator*()), reference>::value,
            "Input operator dereference return type must be exactly reference type");

        // forward constructor
        template<typename... Types>
        _input_iterator_adapter(Types&&... values): Impl(std::forward<Types>(values)...)
        {}

        // Use using-declaration for intoducing operator++() and operator++(int) in this class to avoid abmigious name lookup for this operators
        using _preIncrementDefault<Impl, iterator_t>::operator++;
        using _postIncrementDefault<Impl, iterator_t, value_type>::operator++;
    };

}

template<class Impl, typename value_type_t = typename iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = typename iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = typename iterator_impl_def::get_def_pointer_t<Impl>,
         typename differnce_t = std::ptrdiff_t>
struct input_iterator: iterator_impl_def::_input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, differnce_t,
        input_iterator<Impl, value_type_t, reference_t, pointer_t>>
{
    // forward constructor
    template<typename... Types>
    input_iterator(Types&&... values):
        iterator_impl_def::_input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, differnce_t,
            input_iterator<Impl, value_type_t, reference_t, pointer_t>>(std::forward<Types>(values)...)
    {}
};

