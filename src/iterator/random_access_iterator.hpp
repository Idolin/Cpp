#pragma once

#include "./bidirectional_iterator.hpp"
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
    struct _random_access_iterator_adapter:
            iterator_impl_def::_bidirectional_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>,
            iterator_impl_def::_additionDefault<Impl, iterator_t>,
            iterator_impl_def::_subtractionDefault<Impl, iterator_t>,
            iterator_impl_def::_iteratorSubtractionDefault<Impl, iterator_t>,
            iterator_impl_def::_arrayIndexDefault<Impl, iterator_t>,
            iterator_impl_def::_comparisonsDefault<Impl, iterator_t>
    {
        typedef std::random_access_iterator_tag iterator_category;

        // forward constructor
        template<typename... Types>
        _random_access_iterator_adapter(Types&&... values):
            iterator_impl_def::_bidirectional_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>(std::forward<Types>(values)...)
        {}


    };

}

template<class Impl, typename value_type_t = typename iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = typename iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = typename iterator_impl_def::get_def_pointer_t<Impl>,
         typename difference_t = typename iterator_impl_def::get_def_difference_t<Impl>>
struct random_access_iterator: iterator_impl_def::_random_access_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
        random_access_iterator<Impl, value_type_t, reference_t, pointer_t, difference_t>>
{
    // forward constructor
    template<typename... Types>
    random_access_iterator(Types&&... values):
        iterator_impl_def::_random_access_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
            random_access_iterator<Impl, value_type_t, reference_t, pointer_t, difference_t>>(std::forward<Types>(values)...)
    {}
};
