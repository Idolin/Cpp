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
            // random access iterator is a bidirectional iterator
            _bidirectional_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>,
            /*
             * adds addition operator+=(difference_t) if not declared based on either
             *  - increment(integral_type) method
             *  - getIndex() and setIndex(integral_type) methods
             */
            _additionAssignmentDefault<Impl, iterator_t, difference_t>,
            /*
             * adds subtraction operator-=(difference_t) if not declared based on either
             *  - decrement(integral_type) method
             *  - getIndex() and setIndex(integral_type) methods
             */
            _subtractionAssignmentDefault<Impl, iterator_t, difference_t>,
            // adds subtraction operator-(const It&) if not declared based on getIndex() and setIndex(integral_type) methods
            _iteratorSubtractionDefault<Impl, iterator_t, difference_t>,
            /*
             * adds subscription operator[integral_type] if not declared based on derefernce operator*() and either
             *  - non-member function operator+(const It&, integralt_type)
             *  - operator+(integral_type)
             *  - increment(integral_type) method
             *  - getIndex() and setIndex(integral_type) methods
             */
            _subscriptDefault<Impl, iterator_t, reference_t, difference_t>,
            // adds operator<(const It&) if not declared (and not declared non-member function operator<(const It&, const It&)) based on getIndex() method
            _comparisonLessDefault<Impl, iterator_t>,
            /*
             * adds operator<=(const It&) if not declared (and not declared non-member function operator<=(const It&, const It&)) based on either
             *  - operator<(const It&) and operator==(const It&)
             *  - non-member function operator<(const It&, const It&) and operator==(const It&)
             *  - getIndex() method
             */
            _comparisonLessOrEqualDefault<Impl, iterator_t>,
            /*
             * adds operator>(const It&) if not declared (and not declared non-member function operator>(const It&, const It&)) based on either
             *  - operator<(const It&) and operator==(const It&)
             *  - non-member function operator<(const It&, const It&) and operator==(const It&)
             *  - getIndex() method
             */
            _comparisonGreaterDefault<Impl, iterator_t>,
            /*
             * adds operator>=(const It&) if not declared (and not declared non-member function operator>=(const It&, const It&)) based on either
             *  - operator<(const It&)
             *  - non-member function operator<(const It&, const It&)
             *  - getIndex() method
             */
            _comparisonGreaterOrEqualDefault<Impl, iterator_t>,
            /*
             * adds operator-(integral_type) if not declared (and not declared non-member function operator-(const It&, integral_type)) based on either
             *  - decrement(integral_type) method
             *  - getIndex() and setIndex(integral_type) methods
             */
            _subtractionDefault<Impl, iterator_t, difference_t>,
            /*
             * adds operator+(integral_type) if not declared (and not declared non-member function operator+(const It&, integral_type)) based on either
             *  - increment(integral_type) method
             *  - getIndex() and setIndex(integral_type) methods
             */
            _additionDefault<Impl, iterator_t, difference_t>
    {
        typedef std::random_access_iterator_tag iterator_category;

        // forward constructor
        template<typename... Types>
        _random_access_iterator_adapter(Types&&... values):
            _bidirectional_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>(std::forward<Types>(values)...)
        {}

        /*
         * Use using-declaration for intoducing operator+=(difference_type), operator-=(difference_type), operator-(const It&),
         *  operator-(difference_type) and operator+(difference_type) in this class to avoid abmigious name lookup for this operators
         */
        using _additionAssignmentDefault<Impl, iterator_t, difference_t>::operator+=;
        using _subtractionAssignmentDefault<Impl, iterator_t, difference_t>::operator-=;
        using _iteratorSubtractionDefault<Impl, iterator_t, difference_t>::operator-;
        using _subtractionDefault<Impl, iterator_t, difference_t>::operator-;
        using _additionDefault<Impl, iterator_t, difference_t>::operator+;
    };

    namespace
    {

        template<class Impl, typename value_type_t,
                 typename reference_t,
                 typename pointer_t,
                 typename difference_t,
                 typename iterator_t>
        using _RAI = _random_access_iterator_adapter<Impl, value_type_t, reference_t, pointer_t, difference_t, iterator_t>;

    }

    // function operator+(integral_type, const _random_access_iterator_adapter<It, ...>&) if provided function operator+(integral_type, const It&)
    template<class I, // Impl
             typename V, // value_type
             typename R, // reference
             typename P, // pointer
             typename D, // difference_type
             typename IT> // iterator (CRTP)
    typename std::enable_if_t<is_addable_v<D, I>, // checks if compiles: difference_type + It
            std::remove_reference_t<std::remove_cv_t<IT>>> // return type = IT without const and refernce
        operator+(typename _RAI<I, V, R, P, D, IT>::difference_type value, const _RAI<I, V, R, P, D, IT>& it)
    {
        using T = _RAI<I, V, R, P, D, IT>;

        static_assert(is_addable_v<typename T::difference_type, const typename T::Impl>,
          "Random access iterator It: this addition expression must be valid: integral_type + const It");
        static_assert(std::is_same<typename T::Impl, decltype(std::declval<typename T::difference_type>() + std::declval<typename T::Impl>())>::value,
          "Random access iterator It: this addition expression return type must be exactly It: integral_type + It");
        static_assert(std::is_same<typename T::Impl, decltype(std::declval<typename T::difference_type>() + std::declval<const typename T::Impl>())>::value,
          "Random access iterator It: this addition expression return type must be exactly It: integral_type + const It");
        return value + it.Impl;
    }

    // function operator+(integral_type, const _random_access_iterator_adapter<It, ...>&) if function operator+(integral_type, const It&) not provided
    template<class I, // Impl
             typename V, // value_type
             typename R, // reference
             typename P, // pointer
             typename D, // difference_type
             typename IT> // iterator (CRTP)
    typename std::enable_if_t<!is_addable_v<D, I>, // checks if not compiles: difference_type + It
            std::remove_reference_t<std::remove_cv_t<IT>>> // return type = IT without const and refernce
        operator+(typename _RAI<I, V, R, P, D, IT>::difference_type value, const _RAI<I, V, R, P, D, IT>& it)
    {
        return it + value; // return It + different_type
    }

}

template<class Impl, typename value_type_t = typename iterator_impl_def::get_def_value_t<Impl>,
         typename reference_t = typename iterator_impl_def::get_def_reference_t<Impl>,
         typename pointer_t = typename iterator_impl_def::get_def_pointer_t<Impl>,
         typename difference_t = typename iterator_impl_def::get_def_difference_t<Impl, true>>
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
