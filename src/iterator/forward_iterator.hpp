#pragma once

#include "./iterator_impl.hpp"
#include "./input_iterator.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iterator_impl_def
{

    template<class Impl,
             typename value_type_t,
             typename reference_t,
             typename pointer_t,
             typename difference_t,
             typename iterator_t>
    struct _forward_iterator_adapter:
            // forward iterator is an input iterator
            _input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>
    {
    private:
        typedef decltype(std::declval<_postIncrementDefault<Impl, iterator_t, void>>().operator++(0)) post_increment_return_t;

    public:
        typedef std::forward_iterator_tag iterator_category;

        static_assert(std::is_default_constructible<Impl>::value,
            "Forward iterator must be default constructible");
        static_assert(std::is_same<reference_t, std::add_lvalue_reference_t<value_type_t>>::value ||
            std::is_same<reference_t, std::add_lvalue_reference_t<std::add_const_t<value_type_t>>>::value,
            "Forward iterator reference type must be exactly value_type& or const value_type&");

        static_assert(std::is_same<post_increment_return_t, iterator_t>::value,
            "Forward iterator It post-increment return type must be exactly It");

        // forward constructor
        template<typename... Types>
        _forward_iterator_adapter(Types&&... values):
            _input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>(std::forward<Types>(values)...)
        {}
    };

    template<class Impl,
             typename value_type_t,
             typename reference_t,
             typename pointer_t,
             typename difference_t>
    struct _forward_iterator_: _forward_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
            _forward_iterator_<Impl, value_type_t, reference_t, pointer_t, difference_t>>
    {
        // forward constructor
        template<typename... Types>
        _forward_iterator_(Types&&... values):
            _forward_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
                _forward_iterator_<Impl, value_type_t, reference_t, pointer_t, difference_t>>(std::forward<Types>(values)...)
        {}
    };

}

template<class Impl, typename value_type_t = iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = iterator_impl_def::get_def_pointer_t<Impl>,
         typename difference_t = iterator_impl_def::get_def_difference_t<Impl>>
using forward_iterator = std::conditional_t<
    // If Impl is already forward iterator with typedefs matching given typenames,
    is_valid_stl_forward_iterator_v<Impl> && has_desired_iterator_typedefs_v<Impl, value_type_t, reference_t, pointer_t, difference_t>,
    // then no need to wrap it, just return Impl itself,
    Impl,
    // otherwise return forward iterator adapter class
    iterator_impl_def::_forward_iterator_<Impl, value_type_t, reference_t, pointer_t, difference_t>>;
