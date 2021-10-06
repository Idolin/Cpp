#pragma once

#include "./iterator_impl.hpp"
#include "../template/type_tags.hpp"
#include "../iterator/iterator_check.hpp"

#include <iterator>
#include <type_traits>
#include <utility>

namespace iterator_impl_def
{

    template<class Impl>
    struct _output_iterator_adapter: public Impl,
            /*
             * adds pre-increment operator++() if not declared based on either
             *  - increment() method
             *  - getIndex() and setIndex(integral_type) methods
             */
            iterator_impl_def::_preIncrementDefault<Impl, _output_iterator_adapter<Impl>>,
            /*
             * adds post-increment operator++(int) if not declared based on either
             *  - pre-increment operator++()
             *  - increment() method
             *  - getIndex() and setIndex(integral_type) methods
             */
            iterator_impl_def::_postIncrementDefault<Impl, _output_iterator_adapter<Impl>, void>
    {
    private:
        typedef _output_iterator_adapter<Impl> self;
        typedef decltype(std::declval<iterator_impl_def::_postIncrementDefault<Impl, self, void>>().operator++(0)) post_increment_return_t;

    public:
        typedef std::output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void reference;
        typedef void pointer;
        typedef void difference_type;

        static_assert(is_dereferencable_v<Impl>, "Iterator must be dereferencable");

        // if post_increment_return_t == self, then calling is_convertible on post_increment_return_t(self) is undefined behaviour, as self is incomplete type
        static_assert(std::is_same<post_increment_return_t, self>::value || std::is_convertible<post_increment_return_t, const Impl&>::value,
                "Output iterator post-increment return type must be convertible to const It&");

        // forward constructor
        template<typename... Types>
        _output_iterator_adapter(Types&&... values): Impl(std::forward<Types>(values)...)
        {}

        // Use using-declaration for intoducing operator++() and operator++(int) in this class to avoid abmigious name lookup for this operators
        using iterator_impl_def::_preIncrementDefault<Impl, self>::operator++;
        using iterator_impl_def::_postIncrementDefault<Impl, self, void>::operator++;
    };

}

template<class Impl>
using output_iterator = std::conditional_t<is_valid_stl_output_iterator_v<Impl>, Impl, iterator_impl_def::_output_iterator_adapter<Impl>>;
