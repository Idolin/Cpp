#pragma once

#include "./iterator_impl.hpp"
#include "./iterator_check.hpp"
#include "../template/type_tags.hpp"

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>


namespace iterator_impl_def
{

    namespace
    {

        template<bool enable, typename T>
        struct _GetWrappedTypeIf
        {
            typedef T type;

            // adds const to T type if it's not an rvalue_reference(&&) (T& -> const T&, T -> const T, T&& -> T&&)
            typedef std::conditional_t<std::is_rvalue_reference<T>::value,
                T,
                add_const_ignore_reference_t<T>> const_ref_type;

            // adds const to T* type (T* -> const T*)
            typedef add_const_ignore_reference_and_pointer_t<T> const_ptr_type;
        };

        template<typename T>
        struct _GetWrappedTypeIf<true, T>
        {
            typedef typename T::type type;
            typedef typename T::type const_ref_type;
            typedef typename T::type const_ptr_type;
        };

        struct _TypeWrapperDefaultTag
        {};

        template<typename T>
        struct _TypeWrapperDefault: _TypeWrapperDefaultTag
        {
            typedef T type;
        };

        template<typename T>
        using _wrapped_type_t = typename _GetWrappedTypeIf<std::is_base_of<_TypeWrapperDefaultTag, T>::value, T>::type;

        template<typename T>
        using _wrapped_const_ref_type_t = typename _GetWrappedTypeIf<std::is_base_of<_TypeWrapperDefaultTag, T>::value, T>::const_ref_type;

        template<typename T>
        using _wrapped_const_ptr_type_t = typename _GetWrappedTypeIf<std::is_base_of<_TypeWrapperDefaultTag, T>::value, T>::const_ref_type;

    }

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

        static_assert(has_dereference_operator_v<Impl>,
            "Iterator must be dereferencable");
        static_assert(std::is_integral<difference_type>::value,
            "Iterator difference_type better to be integral in order to provide type for algortihms like std::distance");
        static_assert(std::is_signed<difference_type>::value,
            "Iterator difference_type must be signed to represent negative offset between iterators");
        static_assert(std::is_convertible<reference, value_type>::value,
            "Input iterator reference type needs to be convertible to value_type");

        // forward constructor
        template<typename... Types>
        _input_iterator_adapter(Types&&... values): Impl(std::forward<Types>(values)...)
        {}

        // Use using-declaration for intoducing operator++(), operator++(int) and operator->() in this class to avoid abmigious name lookup for this operators
        using _arrowDefault<Impl, iterator_t, pointer_t>::operator->;
        using _preIncrementDefault<Impl, iterator_t>::operator++;
        using _postIncrementDefault<Impl, iterator_t, value_type>::operator++;

        /*
         * Input iterator operator*() must return reference type, but Impl allowed to return type convertible to reference, so
         *  we overriding Impl::operator*() to match this requirement
         */
        reference operator*() const
        {
            static_assert(std::is_convertible<decltype(*std::declval<Impl>()), reference>::value,
                "Input iterator adapter: make dereference return type convertible to reference type (Input operator dereference return type must be exactly reference type)");

            return *(*static_cast<const Impl*>(this));
        }
    };

    template<class Impl, typename value_type_t,
             typename reference_t,
             typename pointer_t,
             typename difference_t>
    struct _input_iterator_: _input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
            _input_iterator_<Impl, value_type_t, reference_t, pointer_t, difference_t>>
    {
        // forward constructor
        template<typename... Types>
        _input_iterator_(Types&&... values):
            _input_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t,
                _input_iterator_<Impl, value_type_t, reference_t, pointer_t, difference_t>>(std::forward<Types>(values)...)
        {}
    };

}

template<class Impl, typename value_type_t = iterator_impl_def::get_def_value_t<Impl>,
         // _TypeWrapperDefault used mainly to check whether type was explicitly stated or not
         typename reference_t = iterator_impl_def::_TypeWrapperDefault<iterator_impl_def::get_def_reference_t<Impl>>,
         typename pointer_t = iterator_impl_def::_TypeWrapperDefault<iterator_impl_def::get_def_pointer_t<Impl>>,
         typename difference_t = iterator_impl_def::get_def_difference_t<Impl>>
using input_iterator = std::conditional_t<is_valid_stl_input_iterator_v<Impl> && // if Impl is already input pointer
    // with typedefs matching given typenames,
    has_desired_iterator_typedefs_v<Impl, value_type_t, iterator_impl_def::_wrapped_type_t<reference_t>,
        iterator_impl_def::_wrapped_type_t<pointer_t>, difference_t>,
    // then no need to wrap it, just return Impl itself,
    Impl,
    // otherwise return input iterator adapter class (reference and pointer referred types will be const by default (const T& and const T*))
    iterator_impl_def::_input_iterator_<Impl, value_type_t, iterator_impl_def::_wrapped_const_ref_type_t<reference_t>,
        iterator_impl_def::_wrapped_const_ptr_type_t<pointer_t>, difference_t>>;

