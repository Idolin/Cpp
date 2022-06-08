#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

#include "../other/defdef.h"
#include "../template/type_tags.hpp"
#include "../other/platform_specific.hpp"

namespace iterator
{

        struct AnyType
        {};

}

/*
 * Do all possible compile-time checks to ensure that IT is sutisfies iterator requirements for it's iterator_type
 * It provides:
 *  - typedefs validation
 *  - expression validation (existance and return types)
 *  - few other validations (default constructor existance, etc.)
 */
template<typename IT, bool throw_error = false>
struct is_valid_stl_iterator;

template<typename IT, bool throw_error = false>
struct is_valid_stl_output_iterator;

template<typename IT, bool throw_error = false>
struct is_valid_stl_input_iterator;

template<typename IT, bool throw_error = false>
struct is_valid_stl_forward_iterator;

template<typename IT, bool throw_error = false>
struct is_valid_stl_bidirectional_iterator;

template<typename IT, bool throw_error = false>
struct is_valid_stl_random_access_iterator;

template<typename IT, bool throw_error = false>
struct is_valid_stl_const_iterator;

template<typename IT, bool throw_error = false>
struct is_valid_stl_mutable_iterator;

template<typename IT, typename DesiredValueType = iterator::AnyType, typename DesiredReferenceType = iterator::AnyType,
         typename DesiredPointerType = iterator::AnyType, typename DesiredDifferenceType = iterator::AnyType, bool throw_error = false>
struct has_desired_iterator_typedefs;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_iterator_v = is_valid_stl_iterator<IT, throw_error>::value;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_output_iterator_v = is_valid_stl_output_iterator<IT, throw_error>::value;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_input_iterator_v = is_valid_stl_input_iterator<IT, throw_error>::value;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_forward_iterator_v = is_valid_stl_forward_iterator<IT, throw_error>::value;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_bidirectional_iterator_v = is_valid_stl_bidirectional_iterator<IT, throw_error>::value;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_random_access_iterator_v = is_valid_stl_random_access_iterator<IT, throw_error>::value;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_const_iterator_v = is_valid_stl_const_iterator<IT, throw_error>::value;

template<typename IT, bool throw_error = false>
constexpr bool is_valid_stl_mutable_iterator_v = is_valid_stl_mutable_iterator<IT, throw_error>::value;

template<typename IT, typename DesiredValueType = iterator::AnyType, typename DesiredReferenceType = iterator::AnyType,
         typename DesiredPointerType = iterator::AnyType, typename DesiredDifferenceType = iterator::AnyType, bool throw_error = false>
constexpr bool has_desired_iterator_typedefs_v =
        has_desired_iterator_typedefs<IT, DesiredValueType, DesiredReferenceType, DesiredPointerType, DesiredDifferenceType, throw_error>::value;

namespace
{

    #define ASSERT_ENABLE_STATIC_IMPL(check, message) \
        static_assert(!throw_error || check, message); \
        result &= check; \

    #define ASSERT_ENABLE_STATIC(...) ASSERT_ENABLE_STATIC_IMPL((EXCLUDE_LAST_L(__VA_ARGS__)), GET_LAST_L(__VA_ARGS__))


    template<typename T, typename Enable = void>
    struct _valid_iterator_category_typedef
    {
        static constexpr bool value = false;
    };

    template<typename T>
    struct _valid_iterator_category_typedef<T, std::enable_if_t<is_one_of_listed_v<typename T::iterator_category,
            std::output_iterator_tag, std::input_iterator_tag, std::forward_iterator_tag,
            std::bidirectional_iterator_tag, std::random_access_iterator_tag>>>
    {
        static constexpr bool value = true;
    };


    template<typename T, bool throw_error>
    struct _iterator_has_typedefs
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(has_iterator_category_typedef<T>::value,
                                 "Iterator must have member typedef iterator_category");
            ASSERT_ENABLE_STATIC(_valid_iterator_category_typedef<T>::value,
                                 "Iterator iterator_category type must be one of the following: std::output_iterator_tag, "
                                 "std::input_iterator_tag, std::forward_iterator_tag, std::bidirectional_iterator_tag, std::random_access_iterator_tag");
            ASSERT_ENABLE_STATIC(has_value_type_typedef<T>::value,
                                 "Iterator must have member typedef value_type");
            ASSERT_ENABLE_STATIC(has_reference_typedef<T>::value,
                                 "Iterator must have member typedef reference");
            ASSERT_ENABLE_STATIC(has_pointer_typedef<T>::value,
                                 "Iterator must have member typedef pointer");
            ASSERT_ENABLE_STATIC(has_difference_type_typedef<T>::value,
                                 "Iterator must have member typedef difference_type");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_iterator_base_types_check;

    template<typename T, bool throw_error>
    struct _is_iterator_base_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_iterator_base_types_check<true, T, throw_error>
    {
    private:
        typedef std::add_lvalue_reference_t<T> R; // R = T&

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_same<decltype(++std::declval<R>()), R>::value,
                                 "Iterator: pre-increment operator return type must be It& (operator++() -> It&)");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<typename T, bool throw_error>
    struct _is_iterator_base
    {
    private:
        typedef std::add_lvalue_reference_t<T> R; // R = T&

        static constexpr bool _value()
        {
            bool result = _iterator_has_typedefs<T, throw_error>::value;

            ASSERT_ENABLE_STATIC(std::is_copy_constructible<T>::value,
                                 "Iterator must be copy-constructible");
            ASSERT_ENABLE_STATIC(std::is_copy_assignable<T>::value,
                                 "Iterator must be copy-assignable");
            ASSERT_ENABLE_STATIC(std::is_destructible<T>::value,
                                 "Iterator must be destructible");
            ASSERT_ENABLE_STATIC(cmt::is_swappable<R>::value,
                                 "Iterator: lvalue of type It must be swappable");

            ASSERT_ENABLE_STATIC(is_dereferencable_v<R>,
                                 "Iterator: lvalue of type It must be dereferencable (operator*())");
            ASSERT_ENABLE_STATIC(has_pre_increment_operator_v<R>,
                                 "Iterator: lvalue of type It must be incrementable (operator++())");

            return result;
        }

    public:
        static constexpr bool value = _is_iterator_base_types_check<_value(), T, throw_error>::value;
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_output_iterator_nested_types_check;

    template<typename T, bool throw_error>
    struct _is_output_iterator_nested_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_output_iterator_nested_types_check<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!is_const_ignore_reference_v<decltype(*(std::declval<T>()++))>,
                                 "Output iterator post-increment can't return const value on derefernce (as it must be assignable to) (*It++ = ...)");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<bool enable, typename T, bool throw_error>
    struct _is_output_iterator_types_check;

    template<typename T, bool throw_error>
    struct _is_output_iterator_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_output_iterator_types_check<true, T, throw_error>
    {
    private:
        typedef std::add_lvalue_reference_t<T> R; // R = T&
        typedef add_const_ignore_reference_t<R> CR; // CR = const T&

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!is_const_ignore_reference_v<decltype(*std::declval<T>())>,
                                 "Output iterator can't return const value on derefernce (as it must be assignable to) (*It = ...)");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(++std::declval<T>()), R>::value,
                                 "Output iterator: pre-increment operator return type must be It& (operator++() -> It&)");

            ASSERT_ENABLE_STATIC(std::is_convertible<decltype(std::declval<T>()++), CR>::value,
                                 "Output iterator: post-increment operator return type must be convertible to const It& (operator++(int) -> X (convertible to const It&))");
            ASSERT_ENABLE_STATIC(has_dereference_operator_v<decltype(std::declval<T>()++)>,
                                 "Output iterator: post-increment return type must be dereferencable (operator++(int) -> X (dereferencable (operator*()))");

            return result;
        }

    public:
        static constexpr bool value = _is_output_iterator_nested_types_check<_value(), T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _is_output_iterator
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_class<T>::value,
                                 "Output iterator must be pointer or class type"); // pointers checked separately

            ASSERT_ENABLE_STATIC(!std::is_same<typename T::iterator_category, std::input_iterator_tag>::value,
                                 "Output iterator can't have input_iterator_tag as it's iterator_category");

            ASSERT_ENABLE_STATIC(has_dereference_operator_v<T>,
                                 "Output iterator must be dereferencable (operator*())");
            ASSERT_ENABLE_STATIC(has_pre_increment_operator_v<T>,
                                 "Output iterator must be pre-incrementable (operator++())");
            ASSERT_ENABLE_STATIC(has_post_increment_operator_v<T>,
                                 "Output iterator must have post-increment operator (operator++(int))");

            // we can't check if dereferenced output iterator is assignable as we don't know valid writable types for this output iterator

            return result;
        }

    public:
        static constexpr bool value = _is_output_iterator_types_check<_value(), T, throw_error>::value;
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_input_iterator_nested_types_check;

    template<typename T, bool throw_error>
    struct _is_input_iterator_nested_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_input_iterator_nested_types_check<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_convertible<decltype(*(std::declval<T>()++)), typename T::value_type>::value,
                                 "Input iterator: dereferenced post-increment operator return type must be convertable to value_type (*It++ -> X (convertible to It::value_type))");
            return result;
        }

    public:
        static constexpr bool value = _value();
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_input_iterator_types_check;

    template<typename T, bool throw_error>
    struct _is_input_iterator_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_input_iterator_types_check<true, T, throw_error>
    {
    private:
        typedef std::add_lvalue_reference_t<T> R; // R = T&
        typedef std::add_const_t<T> C; // C = const T

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_convertible<decltype(std::declval<T>() == std::declval<T>()), bool>::value,
                                 "Input iterator must be equally comparable (operator==(const It&) -> B (implicitly convertable to bool))");
            ASSERT_ENABLE_STATIC(std::is_convertible<decltype(std::declval<C>() == std::declval<C>()), bool>::value,
                                 "Input iterator const It must be equally comparable (const It == const It -> B (implicitly convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_convertible<decltype(std::declval<C>() == std::declval<T>()), bool>::value,
                                 "Input iterator const It must be equally comparable (const It == It -> B (implicitly convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_convertible<decltype(std::declval<T>() == std::declval<C>()), bool>::value,
                                 "Input iterator const It must be equally comparable (It == const It -> B (implicitly convertable to bool)))");

            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() != std::declval<T>())>::value,
                                 "Input iterator must have inequality operator (operator!=(const It&) -> B (contextually convertable to bool))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() != std::declval<C>())>::value,
                                 "Input iterator const It must be inequality comparable (const It != const It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() != std::declval<T>())>::value,
                                 "Input iterator const It must be inequality comparable (const It != It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() != std::declval<C>())>::value,
                                 "Input iterator const It must be inequality comparable (It != const It -> B (contextually convertable to bool)))");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(*std::declval<T>()), typename T::reference>::value,
                                 "Input iterator derefernce operator must return reference type (operator*() -> It::reference)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(*std::declval<C>()), typename T::reference>::value,
                                 "Input iterator(const) derefernce operator must return reference type (*(const It) -> It::reference)");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>().operator->()), typename T::pointer>::value,
                                 "Input iterator arrow-like operator must return pointer type (operator->() -> It::pointer)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<C>().operator->()), typename T::pointer>::value,
                                 "Input iterator(const) arrow operator must return pointer type ((const It).operator->() -> It::pointer)");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(++std::declval<T>()), R>::value,
                                 "Input iterator pre-increment operator must return It& (operator++() -> It&)");

            ASSERT_ENABLE_STATIC(has_dereference_operator_v<decltype(std::declval<T>()++)>,
                                 "Input iterator post-increment operator must return dereferencable type (*It++ -> X)");

            return result;
        }

    public:
        static constexpr bool value = _is_input_iterator_nested_types_check<_value(), T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _is_input_iterator
    {
    private:
        typedef std::add_const_t<T> C; // C = const T

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_same<typename T::iterator_category, std::output_iterator_tag>::value,
                                 "Input iterator can't have output_iterator_tag as it's iterator_category");
            ASSERT_ENABLE_STATIC(!std::is_same<typename T::value_type, void>::value,
                                 "Input iterator value_type can't be void");
            ASSERT_ENABLE_STATIC(!std::is_same<typename T::reference, void>::value,
                                 "Input iterator reference can't be void");
            ASSERT_ENABLE_STATIC(std::is_pointer<typename T::pointer>::value,
                                 "Input iterator pointer type must be a pointer (operator->() -> It::pointer)");
            ASSERT_ENABLE_STATIC(std::is_signed<typename T::difference_type>::value,
                                 "Input iterator difference_type must be signed intergral type");
            ASSERT_ENABLE_STATIC(std::is_convertible<typename T::reference, typename T::value_type>::value,
                                 "Input iterator reference must be convertible to value_type");

            ASSERT_ENABLE_STATIC(has_equal_operator_v<T>,
                                 "Input iterator must be equally comparable (operator==(const It&))");
            ASSERT_ENABLE_STATIC(has_not_equal_operator_v<T>,
                                 "Input iterator must have inequality operator (operator!=(const It&))");

            ASSERT_ENABLE_STATIC(has_dereference_operator_v<T>,
                                 "Input iterator must be dereferncable (operator*())");
            ASSERT_ENABLE_STATIC(has_dereference_operator_v<C>,
                                 "Input iterator(const) must be dereferncable (operator*() const)");

            ASSERT_ENABLE_STATIC(has_arrow_operator_v<T>,
                                 "Input iterator must have arrow operator (operator->())");
            ASSERT_ENABLE_STATIC(has_arrow_operator_v<C>,
                                 "Input iterator must have arrow operator (operator->() const)");

            ASSERT_ENABLE_STATIC(has_pre_increment_operator_v<T>,
                                 "Input iterator must be pre-incrementable (operator++())");

            ASSERT_ENABLE_STATIC(has_post_increment_operator_v<T>,
                                 "Input iterator must be post-incrementable (operator++(int))");

            return result;
        }

    public:
        static constexpr bool value = _is_input_iterator_types_check<_value(), T, throw_error>::value;
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_forward_iterator_nested_types_check;

    template<typename T, bool throw_error>
    struct _is_forward_iterator_nested_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_forward_iterator_nested_types_check<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_same<decltype(*(std::declval<T>()++)), typename T::reference>::value,
                                 "Forward iterator: dereferenced post-increment operator return type must be reference (*It++ -> It::reference)");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<bool enable, typename T, bool throw_error>
    struct _is_forward_iterator_types_check;

    template<typename T, bool throw_error>
    struct _is_forward_iterator_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_forward_iterator_types_check<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>()++), T>::value,
                                 "Forward iterator post-increment operator must return It (operator++(int) -> It)");

            return result;
        }

    public:
        static constexpr bool value = _is_forward_iterator_nested_types_check<_value(), T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _is_forward_iterator
    {
    private:
        typedef std::add_lvalue_reference_t<typename T::value_type> RV; // RV = T::value_type&
        typedef add_const_ignore_reference_t<RV> CRV; // CRV = const T::value_type&

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_same<typename T::iterator_category, std::input_iterator_tag>::value,
                                 "Forward iterator can't have input_iterator_tag as it's iterator_category");

            ASSERT_ENABLE_STATIC(std::is_default_constructible<T>::value,
                                 "Forward iterator must be default constructible");

            ASSERT_ENABLE_STATIC(std::is_same<typename T::reference, RV>::value || std::is_same<typename T::reference, CRV>::value,
                                 "Forward iterator reference type must be either value_type& ot const value_type&");

            ASSERT_ENABLE_STATIC(has_post_increment_operator_v<T>,
                                 "Forward iterator must be post-incrementable (operator++(int))");



            return result;
        }

    public:
        static constexpr bool value = _is_forward_iterator_types_check<_value(), T, throw_error>::value;
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_bidirectional_iterator_nested_types_check;

    template<typename T, bool throw_error>
    struct _is_bidirectional_iterator_nested_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_bidirectional_iterator_nested_types_check<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_same<decltype(*(std::declval<T>()--)), typename T::reference>::value,
                                 "Bidirectional iterator: dereferenced post-decrement operator return type must be reference (*It-- -> It::reference)");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<bool enable, typename T, bool throw_error>
    struct _is_bidirectional_iterator_types_check;

    template<typename T, bool throw_error>
    struct _is_bidirectional_iterator_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_bidirectional_iterator_types_check<true, T, throw_error>
    {
    private:
        typedef std::add_lvalue_reference_t<T> R; // R = T&
        typedef std::add_const_t<R> CR; // CR = const T&

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_same<decltype(--std::declval<T>()), R>::value,
                                 "Bidirectional iterator pre-decrement operator must return It& (operator--() -> It&)");

            ASSERT_ENABLE_STATIC(std::is_convertible<decltype(std::declval<T>()--), CR>::value,
                                 "Bidirectional iterator post-decrement operator must return type convertible to const It& (operator--(int) -> X (convertible to const It&))");
            ASSERT_ENABLE_STATIC(has_dereference_operator_v<decltype(std::declval<T>()--)>,
                                 "Bidirectional iterator post-decrement operator must return dereferencable type (*It-- -> X)");


            return result;
        }

    public:
        static constexpr bool value = _is_bidirectional_iterator_nested_types_check<_value(), T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _is_bidirectional_iterator
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_same<typename T::iterator_category, std::forward_iterator_tag>::value,
                                 "Bidirectional iterator can't have forward_iterator_tag as it's iterator_category");

            ASSERT_ENABLE_STATIC(has_pre_decrement_operator_v<T>,
                                 "Bidirectional iterator must be pre-decrementable (operator--())");
            ASSERT_ENABLE_STATIC(has_post_decrement_operator_v<T>,
                                 "Bidirectional iterator must be post-decrementable (operator--(int))");

            return result;
        }

    public:
        static constexpr bool value = _is_bidirectional_iterator_types_check<_value(), T, throw_error>::value;
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_random_access_iterator_types_check;

    template<typename T, bool throw_error>
    struct _is_random_access_iterator_types_check<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<typename T, bool throw_error>
    struct _is_random_access_iterator_types_check<true, T, throw_error>
    {
    private:
        typedef std::add_lvalue_reference_t<T> R; // R = T&
        typedef std::add_const_t<T> C; // C = const T
        typedef typename T::difference_type D; // D = T::difference_type

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>() += std::declval<D>()), R>::value,
                                 "Random access iterator addition assignment operator must return It& (operator+=(It::difference_type) -> It&)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>() -= std::declval<D>()), R>::value,
                                 "Random access iterator subtraction assignment operator must return It& (operator-=(It::difference_type) -> It&)");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>() + std::declval<D>()), T>::value,
                                 "Random access iterator addition operator (for It::difference_type) must return It (operator+(It::difference_type) -> It)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<C>() + std::declval<D>()), T>::value,
                                 "Random access iterator(const) addition operator (for It::difference_type) must return It (const It + It::difference_type -> It)");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<D>() + std::declval<T>()), T>::value,
                                 "Random access iterator addition operator (with It::difference_type on the left side) must return It (operator+(It::difference_type, const It&) -> It)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<D>() + std::declval<C>()), T>::value,
                                 "Random access iterator(const) addition operator (with It::difference_type on the left side) must return It (It::difference_type + const It -> It)");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>() - std::declval<D>()), T>::value,
                                 "Random access iterator subtraction operator (for It::difference_type) must return It (operator-(It::difference_type) -> It)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<C>() - std::declval<D>()), T>::value,
                                 "Random access iterator(const) subtraction operator (for It::difference_type) must return It (const It - It::difference_type -> It)");

            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>() - std::declval<T>()), D>::value,
                                 "Random access iterator subtraction operator must return It::difference_type (operator-(const It&) -> It::difference_type)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<C>() - std::declval<C>()), D>::value,
                                 "Random access iterator const It subtraction operator must return It::difference_type (const It - const It -> It::difference_type)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<C>() - std::declval<T>()), D>::value,
                                 "Random access iterator const It subtraction operator must return It::difference_type (const It - It -> It::difference_type)");
            ASSERT_ENABLE_STATIC(std::is_same<decltype(std::declval<T>() - std::declval<C>()), D>::value,
                                 "Random access iterator const It subtraction operator must return It::difference_type (It - const It -> It::difference_type)");

            ASSERT_ENABLE_STATIC(std::is_convertible<decltype((std::declval<T>())[std::declval<D>()]), typename T::reference>::value,
                                 "Random access iterator subscription operator must return type convertible to It::reference (operator[](It::difference_type) -> X (convertible to It::reference))");
            ASSERT_ENABLE_STATIC(std::is_convertible<decltype((std::declval<C>())[std::declval<D>()]), typename T::reference>::value,
                                 "Random access iterator (const) subscription operator must return type convertible to It::reference ((const It)[It::difference_type] -> X (convertible to It::reference))");

            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() < std::declval<T>())>::value,
                                 "Random access iterator must have less than comparison operator (operator<(const It&) -> B (contextually convertable to bool))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() < std::declval<C>())>::value,
                                 "Random access iterator const It must be less than comparable (const It < const It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() < std::declval<T>())>::value,
                                 "Random access iterator const It must be less than comparable (const It < It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() < std::declval<C>())>::value,
                                 "Random access iterator const It must be less than comparable (It < const It -> B (contextually convertable to bool)))");

            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() <= std::declval<T>())>::value,
                                 "Random access iterator must have less than or equal to comparison operator (operator<=(const It&) -> B (contextually convertable to bool))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() <= std::declval<C>())>::value,
                                 "Random access iterator const It must be less than or equal to comparable (const It <= const It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() <= std::declval<T>())>::value,
                                 "Random access iterator const It must be less than or equal to comparable (const It <= It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() <= std::declval<C>())>::value,
                                 "Random access iterator const It must be less than or equal to comparable (It <= const It -> B (contextually convertable to bool)))");

            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() > std::declval<T>())>::value,
                                 "Random access iterator must have greater than comparison operator (operator>(const It&) -> B (contextually convertable to bool))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() > std::declval<C>())>::value,
                                 "Random access iterator const It must be greater than comparable (const It > const It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() > std::declval<T>())>::value,
                                 "Random access iterator const It must be greater than comparable (const It > It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() > std::declval<C>())>::value,
                                 "Random access iterator const It must be greater than comparable (It > const It -> B (contextually convertable to bool)))");

            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() >= std::declval<T>())>::value,
                                 "Random access iterator must have greater than or equal to comparison operator (operator>=(const It&) -> B (contextually convertable to bool))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() >= std::declval<C>())>::value,
                                 "Random access iterator const It must be greater than or equal to comparable (const It >= const It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<C>() >= std::declval<T>())>::value,
                                 "Random access iterator const It must be greater than or equal to comparable (const It >= It -> B (contextually convertable to bool)))");
            ASSERT_ENABLE_STATIC(std::is_constructible<bool, decltype(std::declval<T>() >= std::declval<C>())>::value,
                                 "Random access iterator const It must be greater than or equal to comparable (It >= const It -> B (contextually convertable to bool)))");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<typename T, bool throw_error>
    struct _is_random_access_iterator
    {
    private:
        typedef std::add_lvalue_reference_t<T> R; // R = T&
        typedef std::add_const_t<T> C; // C = const T
        typedef typename T::difference_type D; // D = T::difference_type

        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_same<typename T::iterator_category, std::bidirectional_iterator_tag>::value,
                                 "Random access iterator can't have bidirectional_iterator_tag as it's iterator_category");

            ASSERT_ENABLE_STATIC(has_addition_assignment_operator_v<T, D>,
                                 "Random access iterator must have addition assignment operator (operator+=(It::difference_type))");
            ASSERT_ENABLE_STATIC(has_subtraction_assignment_operator_v<T, D>,
                                 "Random access iterator must have subtraction assignment operator (operator-=(It::difference_type))");

            ASSERT_ENABLE_STATIC(has_addition_operator_v<T, D>,
                                 "Random access iterator must have addition operator (for It::difference_type) (operator+(It::difference_type))");
            ASSERT_ENABLE_STATIC(has_addition_operator_v<C, D>,
                                 "Random access iterator(const) must have addition operator (for It::difference_type) (const It + It::difference_type)");

            ASSERT_ENABLE_STATIC(has_addition_operator_v<D, T>,
                                 "Random access iterator must have addition operator (with It::difference_type on the left side) (operator+(It::difference_type, const It&))");
            ASSERT_ENABLE_STATIC(has_addition_operator_v<D, C>,
                                 "Random access iterator(const) must have addition operator (with It::difference_type on the left side) (It::difference_type + const It)");

            ASSERT_ENABLE_STATIC(has_subtraction_operator_v<T, D>,
                                 "Random access iterator must have subtraction operator (for It::difference_type) (operator-(It::difference_type))");
            ASSERT_ENABLE_STATIC(has_subtraction_operator_v<C, D>,
                                 "Random access iterator(const) must have subtraction operator (for It::difference_type) (const It - It::difference_type)");

            ASSERT_ENABLE_STATIC(has_subtraction_operator_v<T, T>,
                                 "Random access iterator must have subtraction operator (operator-(const It&))");
            ASSERT_ENABLE_STATIC(has_subtraction_operator_v<C, C>,
                                 "Random access iterator(const) must have subtraction operator (const It - const It)");

            ASSERT_ENABLE_STATIC(has_subscript_operator_v<T, D>,
                                 "Random access iterator must have subscription operator (operator[](It::difference_type))");
            ASSERT_ENABLE_STATIC(has_subscript_operator_v<C, D>,
                                 "Random access iterator(const) must have subscription operator (operator[](It::difference_type) const)");

            ASSERT_ENABLE_STATIC(has_less_operator_v<T>,
                                 "Random access iterator must have less than operator (operator<(const It&))");
            ASSERT_ENABLE_STATIC(has_less_operator_v<C>,
                                 "Random access iterator(const) must have less than operator (operator<(const It&) const)");

            ASSERT_ENABLE_STATIC(has_less_or_equal_operator_v<T>,
                                 "Random access iterator must have less than or equal to operator (operator<=(const It&))");
            ASSERT_ENABLE_STATIC(has_less_or_equal_operator_v<C>,
                                 "Random access iterator(const) must have less than or equal to operator (operator<=(const It&) const)");

            ASSERT_ENABLE_STATIC(has_greater_operator_v<T>,
                                 "Random access iterator must have greater than operator (operator>(const It&))");
            ASSERT_ENABLE_STATIC(has_greater_operator_v<C>,
                                 "Random access iterator(const) must have greater than operator (operator>(const It&) const)");

            ASSERT_ENABLE_STATIC(has_greater_or_equal_operator_v<T>,
                                 "Random access iterator must have greater than or equal to operator (operator>=(const It&))");
            ASSERT_ENABLE_STATIC(has_greater_or_equal_operator_v<C>,
                                 "Random access iterator(const) must have greater than or equal to operator (operator>=(const It&) const)");

            return result;
        }

    public:
        static constexpr bool value = _is_random_access_iterator_types_check<_value(), T, throw_error>::value;
    };


    template<typename T, bool throw_error>
    struct _is_pointer_iterator
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_same<void*, typename std::remove_cv<T>::type>::value,
                                 "void* is not a valid iterator");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };


    template<bool enable, typename T, bool throw_error>
    struct _is_non_const_pointer_iterator;

    template<typename T, bool throw_error>
    struct _is_non_const_pointer_iterator<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_const<typename std::remove_pointer<T>::type>::value,
                                 "Pointer to const type is not a valid output iterator");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<typename T, bool throw_error>
    struct _is_non_const_pointer_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_pointer_iterator;

    template<typename T, bool throw_error>
    struct _check_is_pointer_iterator<true, T, throw_error>
    {
        static constexpr bool value = _is_pointer_iterator<T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_pointer_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_pointer_output_iterator;

    template<typename T, bool throw_error>
    struct _check_is_pointer_output_iterator<true, T, throw_error>
    {
        static constexpr bool value = _is_non_const_pointer_iterator<_is_pointer_iterator<T, throw_error>::value, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_pointer_output_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_const_pointer_iterator;

    template<typename T, bool throw_error>
    struct _check_is_const_pointer_iterator<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(std::is_const<typename std::remove_pointer<T>::type>::value,
                                 "Expected const iterator, but pointer to non-const type is a mutable iterator");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<typename T, bool throw_error>
    struct _check_is_const_pointer_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_mutable_pointer_iterator;

    template<typename T, bool throw_error>
    struct _check_is_mutable_pointer_iterator<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_const<typename std::remove_pointer<T>::type>::value,
                                 "Expected mutable iterator, but pointer to const type is a const iterator");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<typename T, bool throw_error>
    struct _check_is_mutable_pointer_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_output_iterator;

    template<typename T, bool throw_error>
    struct _check_output_iterator<true, T, throw_error>
    {
        static constexpr bool value = _is_output_iterator<T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_output_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_input_iterator;

    template<typename T, bool throw_error>
    struct _check_input_iterator<true, T, throw_error>
    {
        static constexpr bool value = _is_input_iterator<T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_input_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_forward_iterator;

    template<typename T, bool throw_error>
    struct _check_forward_iterator<true, T, throw_error>
    {
    private:
        static constexpr bool check_forward = _is_forward_iterator<T, throw_error>::value;
        static constexpr bool to_check_output = std::is_lvalue_reference<typename T::reference>::value &&
                !std::is_const<std::remove_reference_t<typename T::reference>>::value;

    public:
        static constexpr bool value = _check_input_iterator<check_forward, T, throw_error>::value &&
            (!to_check_output || _check_output_iterator<check_forward, T, throw_error>::value);
    };

    template<typename T, bool throw_error>
    struct _check_forward_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_bidirectional_iterator;

    template<typename T, bool throw_error>
    struct _check_bidirectional_iterator<true, T, throw_error>
    {
    private:
         static constexpr bool check_bidirectional = _is_bidirectional_iterator<T, throw_error>::value;

     public:
        static constexpr bool value = _check_forward_iterator<check_bidirectional, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_bidirectional_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_random_access_iterator;

    template<typename T, bool throw_error>
    struct _check_random_access_iterator<true, T, throw_error>
    {
    private:
         static constexpr bool check_random_access = _is_random_access_iterator<T, throw_error>::value;

     public:
        static constexpr bool value = _check_bidirectional_iterator<check_random_access, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_random_access_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<typename C, typename T, bool throw_error>
    struct _check_iterator_specific;

    template<typename T, bool throw_error>
    struct _check_iterator_specific<std::output_iterator_tag, T, throw_error>
    {
        static constexpr bool value = _is_output_iterator<T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_iterator_specific<std::input_iterator_tag, T, throw_error>
    {
        static constexpr bool value = _is_input_iterator<T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_iterator_specific<std::forward_iterator_tag, T, throw_error>
    {
        static constexpr bool value = _check_forward_iterator<true, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_iterator_specific<std::bidirectional_iterator_tag, T, throw_error>
    {
        static constexpr bool value = _check_bidirectional_iterator<true, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_iterator_specific<std::random_access_iterator_tag, T, throw_error>
    {
        static constexpr bool value = _check_random_access_iterator<true, T, throw_error>::value;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_any_iterator_specific;

    template<typename T, bool throw_error>
    struct _check_any_iterator_specific<true, T, throw_error>
    {
        static constexpr bool value = _check_iterator_specific<typename T::iterator_category, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_any_iterator_specific<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_any_iterator;

    template<typename T, bool throw_error>
    struct _check_is_any_iterator<true, T, throw_error>
    {
    private:
        static constexpr bool check_base = _is_iterator_base<T, throw_error>::value;

    public:
        static constexpr bool value = _check_any_iterator_specific<check_base, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_any_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_output_iterator;

    template<typename T, bool throw_error>
    struct _check_is_output_iterator<true, T, throw_error>
    {
        static constexpr bool value = _check_output_iterator<_is_iterator_base<T, throw_error>::value, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_output_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_input_iterator;

    template<typename T, bool throw_error>
    struct _check_is_input_iterator<true, T, throw_error>
    {
        static constexpr bool value = _check_input_iterator<_is_iterator_base<T, throw_error>::value, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_input_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_forward_iterator;

    template<typename T, bool throw_error>
    struct _check_is_forward_iterator<true, T, throw_error>
    {
        static constexpr bool value = _check_forward_iterator<_is_iterator_base<T, throw_error>::value, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_forward_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_bidirectional_iterator;

    template<typename T, bool throw_error>
    struct _check_is_bidirectional_iterator<true, T, throw_error>
    {
        static constexpr bool value = _check_bidirectional_iterator<_is_iterator_base<T, throw_error>::value, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_bidirectional_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_is_random_access_iterator;

    template<typename T, bool throw_error>
    struct _check_is_random_access_iterator<true, T, throw_error>
    {
        static constexpr bool value = _check_random_access_iterator<_is_iterator_base<T, throw_error>::value, T, throw_error>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_random_access_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_const_iterator;

    template<typename T, bool throw_error>
    struct _check_const_iterator<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_same<typename T::iterator_category, std::output_iterator_tag>::value,
                                 "Output iterator cannot be a const iterator");
            ASSERT_ENABLE_STATIC(is_const_ignore_reference_v<typename T::reference>,
                                 "Expected const iterator, so iterator reference type must be a reference to const");
            ASSERT_ENABLE_STATIC(is_const_ignore_pointer_v<typename T::pointer>,
                                 "Expected const iterator, so iterator pointer type should be a pointer to const");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<typename T, bool throw_error>
    struct _check_const_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<bool enable, typename T, bool throw_error>
    struct _check_is_const_iterator;

    template<typename T, bool throw_error>
    struct _check_is_const_iterator<true, T, throw_error>
    {
        static constexpr bool value = _check_const_iterator<
                !std::is_same<typename T::iterator_category, std::input_iterator_tag>::value, T, throw_error>::value ||
            std::is_same<typename T::iterator_category, std::input_iterator_tag>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_const_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error>
    struct _check_mutable_iterator;

    template<typename T, bool throw_error>
    struct _check_mutable_iterator<true, T, throw_error>
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(!std::is_same<typename T::iterator_category, std::input_iterator_tag>::value,
                                 "Input iterator cannot be a mutable iterator");
            ASSERT_ENABLE_STATIC(!is_const_ignore_reference_v<typename T::reference>,
                                 "Expected mutable iterator, so iterator reference type must be a reference to non-const");
            ASSERT_ENABLE_STATIC(!is_const_ignore_pointer_v<typename T::pointer>,
                                 "Expected mutable iterator, so iterator pointer type should be a pointer to non-const");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };

    template<typename T, bool throw_error>
    struct _check_mutable_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };

    template<bool enable, typename T, bool throw_error>
    struct _check_is_mutable_iterator;

    template<typename T, bool throw_error>
    struct _check_is_mutable_iterator<true, T, throw_error>
    {
        static constexpr bool value = _check_const_iterator<
                !std::is_same<typename T::iterator_category, std::output_iterator_tag>::value, T, throw_error>::value ||
            std::is_same<typename T::iterator_category, std::output_iterator_tag>::value;
    };

    template<typename T, bool throw_error>
    struct _check_is_mutable_iterator<false, T, throw_error>
    {
        static constexpr bool value = false;
    };


    template<bool throw_error, typename VD, typename VA, typename RD, typename RA, typename PD, typename PA, typename DD, typename DA>
    struct _check_typedefs_is_desired
    {
    private:
        static constexpr bool _value()
        {
            bool result = true;

            ASSERT_ENABLE_STATIC(is_one_of_listed_v<VD, iterator::AnyType, VA>,
                                 "Iterator value_type didn't match requested type");
            ASSERT_ENABLE_STATIC(is_one_of_listed_v<RD, iterator::AnyType, RA>,
                                 "Iterator reference type didn't match requested type");
            ASSERT_ENABLE_STATIC(is_one_of_listed_v<PD, iterator::AnyType, PA>,
                                 "Iterator pointer type didn't match requested type");
            ASSERT_ENABLE_STATIC(is_one_of_listed_v<DD, iterator::AnyType, DA>,
                                 "Iterator difference_type didn't match requested type");

            return result;
        }

    public:
        static constexpr bool value = _value();
    };


    template<bool enable, typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_pointer_has_desired_typedefs;

    template<typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_pointer_has_desired_typedefs<true, T, throw_error, V, R, P, D>
    {
    private:
        typedef std::remove_pointer_t<T> PointerValueType;

    public:
        static constexpr bool value = _check_typedefs_is_desired<throw_error,
            V, std::conditional_t<cpp_std_version >= 20, std::remove_cv_t<PointerValueType>, PointerValueType>,
            R, std::add_lvalue_reference_t<PointerValueType>,
            P, T,
            D, std::ptrdiff_t>::value;
    };

    template<typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_pointer_has_desired_typedefs<false, T, throw_error, V, R, P, D>
    {
        static constexpr bool value = false;
    };


    template<bool enable, typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_iterator_typedefs_are_desired;

    template<typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_iterator_typedefs_are_desired<true, T, throw_error, V, R, P, D>
    {
        static constexpr bool value = _check_typedefs_is_desired<throw_error,
            V, typename T::value_type,
            R, typename T::reference,
            P, typename T::pointer,
            D, typename T::difference_type>::value;
    };

    template<typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_iterator_typedefs_are_desired<false, T, throw_error, V, R, P, D>
    {
        static constexpr bool value = false;
    };

    template<bool enable, typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_iterator_has_desired_typedefs;

    template<typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_iterator_has_desired_typedefs<true, T, throw_error, V, R, P, D>
    {
        static constexpr bool value =
                _check_iterator_typedefs_are_desired<_iterator_has_typedefs<T, throw_error>::value, T, throw_error, V, R, P, D>::value;
    };

    template<typename T, bool throw_error, typename V, typename R, typename P, typename D>
    struct _check_iterator_has_desired_typedefs<false, T, throw_error, V, R, P, D>
    {
        static constexpr bool value = false;
    };

}

template<typename IT, bool throw_error>
struct is_valid_stl_iterator
{
    static constexpr bool value = _check_is_pointer_iterator<std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_any_iterator<!std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, bool throw_error>
struct is_valid_stl_output_iterator
{
    static constexpr bool value = _check_is_pointer_output_iterator<std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_output_iterator<!std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, bool throw_error>
struct is_valid_stl_input_iterator
{
    static constexpr bool value = _check_is_pointer_iterator<std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_input_iterator<!std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, bool throw_error>
struct is_valid_stl_forward_iterator
{
    static constexpr bool value = _check_is_pointer_iterator<std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_forward_iterator<!std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, bool throw_error>
struct is_valid_stl_bidirectional_iterator
{
    static constexpr bool value = _check_is_pointer_iterator<std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_bidirectional_iterator<!std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, bool throw_error>
struct is_valid_stl_random_access_iterator
{
    static constexpr bool value = _check_is_pointer_iterator<std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_random_access_iterator<!std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, bool throw_error>
struct is_valid_stl_const_iterator
{
private:
    static constexpr bool is_valid_iterator = is_valid_stl_iterator_v<IT, throw_error>;

public:
    static constexpr bool value = _check_is_const_pointer_iterator<is_valid_iterator && std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_const_iterator<is_valid_iterator && !std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, bool throw_error>
struct is_valid_stl_mutable_iterator
{
private:
    static constexpr bool is_valid_iterator = is_valid_stl_iterator_v<IT, throw_error>;

public:
    static constexpr bool value = _check_is_mutable_pointer_iterator<is_valid_iterator && std::is_pointer<IT>::value, IT, throw_error>::value ||
        _check_is_mutable_iterator<is_valid_iterator && !std::is_pointer<IT>::value, IT, throw_error>::value;
};

template<typename IT, typename DesiredValueType, typename DesiredReferenceType,
         typename DesiredPointerType, typename DesiredDifferenceType, bool throw_error>
struct has_desired_iterator_typedefs
{
    static constexpr bool value = _check_pointer_has_desired_typedefs<
            std::is_pointer<IT>::value, IT, throw_error, DesiredValueType, DesiredReferenceType, DesiredPointerType, DesiredDifferenceType>::value ||
        _check_iterator_has_desired_typedefs<
            !std::is_pointer<IT>::value, IT, throw_error, DesiredValueType, DesiredReferenceType, DesiredPointerType, DesiredDifferenceType>::value;
};
