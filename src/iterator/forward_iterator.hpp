#pragma once

#include "./iterator_impl.hpp"
#include "./input_iterator.hpp"

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
            std::is_same<reference_t, std::add_const_t<std::add_lvalue_reference_t<value_type_t>>>::value,
            "Forward iterator reference type must be exactly value_type& or const value_type&");

        static_assert(std::is_same<post_increment_return_t, iterator_t>::value,
            "Forward iterator It post-increment return type must be exactly It");

        // forward constructor
        template<typename... Types>
        _forward_iterator_adapter(Types&&... values):
            _input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>(std::forward<Types>(values)...)
        {}
    };

}

template<class Impl, typename value_type_t = typename iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = typename iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = typename iterator_impl_def::get_def_pointer_t<Impl>,
         typename difference_t = std::ptrdiff_t>
struct forward_iterator: iterator_impl_def::_forward_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
        forward_iterator<Impl, value_type_t, reference_t, pointer_t, difference_t>>
{
    // forward constructor
    template<typename... Types>
    forward_iterator(Types&&... values):
        iterator_impl_def::_forward_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
            forward_iterator<Impl, value_type_t, reference_t, pointer_t, difference_t>>(std::forward<Types>(values)...)
    {}
};
