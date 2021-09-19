#pragma once

#include "./iterator_impl.hpp"

#include <iterator>

namespace iterator_impl_def {

    template<class Impl, typename value_type_t,
             typename reference_t,
             typename pointer_t,
             typename iterator_t>
    struct _input_iterator_adapter: Impl,
            // adds operator!= if not declared based on operator==
            iterator_impl_def::_notEqualDefault<Impl, iterator_t>,
            // adds operator-> if not declared based on operator*
            iterator_impl_def::_arrowDefault<Impl, iterator_t, pointer_t>,
            //adds pre-increment operator++() if not declared based on increment() method
            iterator_impl_def::_preIncrementDefault<Impl, iterator_t>,
            /*
             * adds post-increment operator++(int) if not declared based on either
             *  - pre-increment operator++()
             *  - increment() method
             */
            iterator_impl_def::_postIncrementDefault<Impl, iterator_t>
    {
        typedef std::input_iterator_tag iterator_category;
        typedef value_type_t value_type;
        typedef reference_t reference;
        typedef pointer_t pointer;
        typedef void difference_type;

        // forward constructor
        template<typename... Types>
        _input_iterator_adapter(Types&&... values): Impl(std::forward<Types>(values)...)
        {}

        /*
         * Increment operators implemented here, in upper score,
         *  to ensure they will be called instead of original ones declared in Impl class
         */
        iterator_t& operator++()
        {
            iterator_impl_def::_preIncrementDefault<Impl, iterator_t>::_preIncrement();
            return *static_cast<iterator_t*>(this);
        }

        auto operator++(int)
        {
            return iterator_impl_def::_postIncrementDefault<Impl, iterator_t>::_postIncrement();
        }
    };

}

template<class Impl, typename value_type_t = typename iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = typename iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = typename iterator_impl_def::get_def_pointer_t<Impl>>
struct input_iterator: iterator_impl_def::_input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t,
        input_iterator<Impl, value_type_t, reference_t, pointer_t>>
{
    // forward constructor
    template<typename... Types>
    input_iterator(Types&&... values):
        iterator_impl_def::_input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t,
            input_iterator<Impl, value_type_t, reference_t, pointer_t>>(std::forward<Types>(values)...)
    {}
};

