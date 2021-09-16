#pragma once

#include "./iterator_impl.hpp"

#include <iterator>

template<class Impl, typename value_type_t = typename iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = typename iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = typename iterator_impl_def::get_def_pointer_t<Impl>>
struct input_iterator: iterator_impl_def::_notEqualDefault<Impl>, iterator_impl_def::_arrowDefault<Impl, pointer_t>,
        iterator_impl_def::_postIncrementDefault<Impl>
{
    typedef std::input_iterator_tag iterator_category;
    typedef value_type_t value_type;
    typedef reference_t reference;
    typedef pointer_t pointer;
    typedef void difference_type;

    input_iterator() = default;
};
