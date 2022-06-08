#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include "./t_useful.hpp"

template<typename T>
struct is_numeric
{
    static constexpr bool value = (std::is_integral<T>::value && !std::is_same<std::remove_cv_t<T>, bool>::value) ||
                std::is_floating_point<T>::value;
};

template<typename T>
struct is_cstr
{
    static constexpr bool value = std::is_pointer<T>::value &&
            (std::is_same<
                typename std::remove_cv_t<
                    typename std::remove_reference<typename std::remove_extent<T>::type>::type>, char>::value || 
            std::is_same<typename std::remove_cv_t<typename std::remove_pointer<T>::type>, char>::value);
};

template<typename T = void>
struct is_bit_movable;

template<>
struct is_bit_movable<void>
{};

template<typename T>
struct is_bit_movable
{
    static constexpr bool value = std::is_pod<T>::value || std::is_pointer<T>::value ||
                std::is_base_of<is_bit_movable<>, T>::value;
};

template<typename T = void>
struct is_bit_copyable;

template<>
struct is_bit_copyable<void>
{};

template<typename T>
struct is_bit_copyable
{
    static constexpr bool value = std::is_pod<T>::value || std::is_pointer<T>::value ||
                std::is_base_of<is_bit_copyable<>, T>::value;
};

template<typename Self = void>
struct mergeable;

template<>
struct mergeable<void>
{};

template<typename Self>
struct mergeable: mergeable<void>
{
    virtual void merge(Self&& otr) = 0;
};

template<typename T>
struct is_mergeable
{
    static constexpr bool value = std::is_base_of<mergeable<T>, T>::value;
};


#define IMPL_TYPE_TAGS_STRUCT_CHECK_T(name, ...) \
    template<typename T> \
    struct name<T, __VA_ARGS__> \
    { \
        static constexpr bool value = true; \
    };

#define IMPL_TYPE_TAGS_STRUCT_CHECK(name, ...) \
    template<typename T, typename Enable = void> \
    struct name \
    { \
        static constexpr bool value = false; \
    }; \
    \
    IMPL_TYPE_TAGS_STRUCT_CHECK_T(name, ## __VA_ARGS__) \
    \
    template<typename T> \
    constexpr bool name ## _v = name<T>::value;


#define IMPL_TYPE_TAGS_STRUCT_T_IS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK_T(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_STRUCT_IS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)

#define IMPL_TYPE_TAGS_STRUCT_T_HAS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK_T(has_ ## name, typename cmt::void_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_STRUCT_HAS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK(has_ ## name, typename cmt::void_t<__VA_ARGS__>)


IMPL_TYPE_TAGS_STRUCT_IS(dereferencable, std::is_lvalue_reference<
        decltype(*std::declval<typename std::add_lvalue_reference<T>::type>())>::value)

IMPL_TYPE_TAGS_STRUCT_IS(arrow_accessible, std::is_pointer<decltype(std::declval<T>().operator->())>::value)

IMPL_TYPE_TAGS_STRUCT_IS(pre_incrementable,
        std::is_same<decltype(++std::declval<T>()), std::add_lvalue_reference_t<T>>::value)

IMPL_TYPE_TAGS_STRUCT_CHECK(is_post_incrementable,
        cmt::void_t<decltype(std::declval<T>()++)>)

IMPL_TYPE_TAGS_STRUCT_IS(pre_decrementable,
        std::is_same<decltype(--std::declval<T>()), std::add_lvalue_reference_t<T>>::value)

IMPL_TYPE_TAGS_STRUCT_CHECK(is_post_decrementable,
        cmt::void_t<decltype(std::declval<T>()--)>)


IMPL_TYPE_TAGS_STRUCT_HAS(dereference_operator, decltype(*std::declval<T>()))

IMPL_TYPE_TAGS_STRUCT_HAS(arrow_operator, decltype(std::declval<T>().operator->()))

IMPL_TYPE_TAGS_STRUCT_HAS(pre_increment_operator, decltype(++std::declval<T>()))

IMPL_TYPE_TAGS_STRUCT_HAS(post_increment_operator, decltype(std::declval<T>()++))

IMPL_TYPE_TAGS_STRUCT_HAS(pre_decrement_operator, decltype(--std::declval<T>()))

IMPL_TYPE_TAGS_STRUCT_HAS(post_decrement_operator, void(decltype(std::declval<T>()--)))


IMPL_TYPE_TAGS_STRUCT_HAS(iterator_category_typedef, typename T::iterator_category)
IMPL_TYPE_TAGS_STRUCT_HAS(value_type_typedef, typename T::value_type)
IMPL_TYPE_TAGS_STRUCT_HAS(reference_typedef, typename T::reference)
IMPL_TYPE_TAGS_STRUCT_HAS(pointer_typedef, typename T::pointer)
IMPL_TYPE_TAGS_STRUCT_HAS(difference_type_typedef, typename T::difference_type)



IMPL_TYPE_TAGS_STRUCT_IS(hashable, std::is_integral<T>::value || std::is_pointer<T>::value || is_cstr<T>::value)

IMPL_TYPE_TAGS_STRUCT_T_IS(hashable, std::is_same<
        decltype(std::declval<typename std::add_lvalue_reference<
                typename std::add_const<T>::type>::type>().hash()), uint64_t>::value)


namespace cmt
{
    #if __cplusplus < 201703L

        namespace
        {
            template<typename T, typename Enable = void>
            struct is_swappable_user_provided
            {
                static constexpr bool value = false;
            };

            template<typename T>
            struct is_swappable_user_provided<T,
                cmt::void_t<decltype(swap(std::declval<T&>(), std::declval<T&>()))>>
            {
                static constexpr bool value = true;
            };
        }

        IMPL_TYPE_TAGS_STRUCT_IS(swappable,
                is_swappable_user_provided<T>::value || std::is_move_constructible<T>::value && std::is_move_assignable<T>::value)

        template<typename T, std::size_t N>
        struct is_swappable<T[N]>
        {
            static constexpr bool value = is_swappable<T>::value;
        };

    #else

        using std::is_swappable;

    #endif

}


template<typename T>
struct add_const_ignore_reference
{
    typedef std::add_const_t<T> type;
};

template<typename T>
struct add_const_ignore_reference<T&>
{
    typedef std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>> type;
};

template<typename T>
struct add_const_ignore_reference<T&&>
{
    typedef std::add_rvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>> type;
};

template<typename T>
using add_const_ignore_reference_t = typename add_const_ignore_reference<T>::type;


template<typename T>
struct is_const_ignore_reference
{
    static constexpr bool value = std::is_const<std::remove_reference_t<T>>::value;
};

template<typename T>
constexpr bool is_const_ignore_reference_v = is_const_ignore_reference<T>::value;


template<typename T>
struct add_const_ignore_pointer
{
    typedef std::add_const_t<T> type;
};

template<typename T>
struct add_const_ignore_pointer<T*>
{
    typedef std::add_pointer_t<std::add_const_t<std::remove_pointer_t<T>>> type;
};

template<typename T>
using add_const_ignore_pointer_t = typename add_const_ignore_pointer<T>::type;


template<typename T>
struct is_const_ignore_pointer
{
    static constexpr bool value = std::is_const<std::remove_pointer_t<T>>::value;
};

template<typename T>
constexpr bool is_const_ignore_pointer_v = is_const_ignore_reference<T>::value;


template<typename T>
struct add_const_ignore_reference_and_pointer
{
    typedef std::add_const_t<T> type;
};

template<typename T>
struct add_const_ignore_reference_and_pointer<T&>
{
    typedef std::add_lvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>> type;
};

template<typename T>
struct add_const_ignore_reference_and_pointer<T&&>
{
    typedef std::add_rvalue_reference_t<std::add_const_t<std::remove_reference_t<T>>> type;
};

template<typename T>
struct add_const_ignore_reference_and_pointer<T*>
{
    typedef std::add_pointer_t<std::add_const_t<std::remove_pointer_t<T>>> type;
};

template<typename T>
struct add_const_ignore_reference_and_pointer<T*&>
{
    typedef std::add_pointer_t<std::add_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>> type;
};

template<typename T>
struct add_const_ignore_reference_and_pointer<T*&&>
{
    typedef std::add_pointer_t<std::add_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>> type;
};

template<typename T>
using add_const_ignore_reference_and_pointer_t = typename add_const_ignore_reference_and_pointer<T>::type;


template<typename T>
struct is_const_ignore_reference_and_pointer
{
    static constexpr bool value = std::is_const<std::remove_pointer_t<std::remove_reference_t<T>>>::value;
};

template<typename T>
constexpr bool is_const_ignore_reference_and_pointer_v = is_const_ignore_reference_and_pointer<T>::value;


#define IMPL_TYPE_TAGS_CHECK_TWO_T(name, ...) \
    template<typename T, typename U> \
    struct name<T, U, __VA_ARGS__> \
    { \
        static constexpr bool value = true; \
    };

#define IMPL_TYPE_TAGS_CHECK_TWO_(name, def_type, ...) \
    template<typename T, typename U def_type , typename Enable = void> \
    struct name \
    { \
        static constexpr bool value = false; \
    }; \
    \
    IMPL_TYPE_TAGS_CHECK_TWO_T(name, ## __VA_ARGS__) \
    \
    template<typename T, typename U def_type > \
    constexpr bool name ## _v = name<T, U>::value;

#define IMPL_TYPE_TAGS_CHECK_TWO(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_(name, , ## __VA_ARGS__)

#define IMPL_TYPE_TAGS_CHECK_TWO_COPY(name, def_type, ...) IMPL_TYPE_TAGS_CHECK_TWO_(name, = def_type, ## __VA_ARGS__)


#define IMPL_TYPE_TAGS_CHECK_TWO_T_IS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_T(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_IS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_COPY(is_ ## name, T, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_DEF_IS(name, def_type, ...) IMPL_TYPE_TAGS_CHECK_TWO_COPY(is_ ## name, def_type, typename std::enable_if_t<__VA_ARGS__>)


#define IMPL_TYPE_TAGS_CHECK_TWO_T_HAS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_T(has_ ## name, typename cmt::void_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_HAS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO(has_ ## name, typename cmt::void_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_COPY(has_ ## name, T, typename cmt::void_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_DEF_HAS(name, def_type, ...) IMPL_TYPE_TAGS_CHECK_TWO_COPY(has_ ## name, def_type, typename cmt::void_t<__VA_ARGS__>)


IMPL_TYPE_TAGS_CHECK_TWO_IS(explicitly_convertible,
                            std::is_constructible<U, T>::value && !std::is_convertible<T, U>::value)

// using is_constructible instead of is_convertible to allow operator!= return type contextually convertible to bool as well
IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(less_comparable, std::is_constructible<bool, decltype(std::declval<T>() < std::declval<U>())>::value)

// using is_constructible instead of is_convertible to allow operator!= return type contextually convertible to bool as well
IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(less_or_equal_comparable, std::is_constructible<bool, decltype(std::declval<T>() <= std::declval<U>())>::value)

// using is_constructible instead of is_convertible to allow operator!= return type contextually convertible to bool as well
IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(greater_comparable, std::is_constructible<bool, decltype(std::declval<T>() > std::declval<U>())>::value)

// using is_constructible instead of is_convertible to allow operator!= return type contextually convertible to bool as well
IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(greater_or_equal_comparable, std::is_constructible<bool, decltype(std::declval<T>() >= std::declval<U>())>::value)

// using is_constructible instead of is_convertible to allow operator== return type contextually convertible to bool as well
IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(equal_comparable, std::is_constructible<bool, decltype(std::declval<T>() == std::declval<U>())>::value)

// using is_constructible instead of is_convertible to allow operator!= return type contextually convertible to bool as well
IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(not_equal_comparable, std::is_constructible<bool, decltype(std::declval<T>() != std::declval<U>())>::value)

IMPL_TYPE_TAGS_CHECK_TWO_DEF_IS(subscriptable, int,
       !std::is_same<decltype((std::declval<T>())[std::declval<U>()]), void>::value)

IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(subtractable,
                            !std::is_same<decltype(std::declval<T>() - std::declval<U>()), void>::value)

IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(addable,
                            !std::is_same<decltype(std::declval<T>() + std::declval<U>()), void>::value)

IMPL_TYPE_TAGS_CHECK_TWO_IS(addition_assignable,
                            std::is_same<decltype(std::declval<T>() += std::declval<U>()), std::add_lvalue_reference_t<T>>::value)

IMPL_TYPE_TAGS_CHECK_TWO_IS(subtraction_assignable,
                            std::is_same<decltype(std::declval<T>() -= std::declval<U>()), std::add_lvalue_reference_t<T>>::value)


IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(less_operator, decltype(std::declval<T>() < std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(less_or_equal_operator, decltype(std::declval<T>() <= std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(greater_operator, decltype(std::declval<T>() > std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(greater_or_equal_operator, decltype(std::declval<T>() >= std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(equal_operator, decltype(std::declval<T>() == std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(not_equal_operator, decltype(std::declval<T>() != std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_DEF_HAS(subscript_operator, int, decltype((std::declval<T>())[std::declval<U>()]))

IMPL_TYPE_TAGS_CHECK_TWO_HAS(addition_assignment_operator, decltype(std::declval<T>() += std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_HAS(subtraction_assignment_operator, decltype(std::declval<T>() -= std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(subtraction_operator, decltype(std::declval<T>() - std::declval<U>()))

IMPL_TYPE_TAGS_CHECK_TWO_COPY_HAS(addition_operator, decltype(std::declval<T>() + std::declval<U>()))


template<typename T, typename U>
struct is_same_omit_cv
{
    static constexpr bool value = std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value;
};

template<typename T, typename U>
constexpr bool is_same_omit_cv_v = is_same_omit_cv<T, U>::value;


template<typename T, typename U>
struct is_same_omit_cv_reference
{
    static constexpr bool value = std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, std::remove_cv_t<std::remove_reference_t<U>>>::value;
};

template<typename T, typename U>
constexpr bool is_same_omit_cv_reference_v = is_same_omit_cv_reference<T, U>::value;


template<typename T, bool const_q, bool volatile_q, bool lvalue_reference, bool rvalue_refernce>
struct set_cv_reference
{
private:
    typedef std::remove_cv_t<std::remove_reference_t<T>> B;

    typedef std::conditional_t<const_q, std::add_const_t<B>, B> C;
    typedef std::conditional_t<volatile_q, std::add_volatile_t<C>, C> V;
    typedef std::conditional_t<lvalue_reference, std::add_lvalue_reference_t<V>, V> L;
    typedef std::conditional_t<rvalue_refernce, std::add_rvalue_reference_t<L>, L> R;

public:
    typedef R type;
};

template<typename T, bool const_q, bool volatile_q, bool lvalue_reference, bool rvalue_refernce>
using set_cv_reference_t = typename set_cv_reference<T, const_q, volatile_q, lvalue_reference, rvalue_refernce>::type;

// copying cv-qualifiers and references from F to T type
template<typename F, typename T>
struct copy_cv_refernce
{
    typedef set_cv_reference_t<T, std::is_const<std::remove_reference_t<F>>::value,
        std::is_volatile<std::remove_reference_t<F>>::value, std::is_lvalue_reference<F>::value, std::is_lvalue_reference<F>::value> type;
};

template<typename F, typename T>
using copy_cv_refernce_t = typename copy_cv_refernce<F, T>::type;

template<typename T, typename... Types>
struct is_one_of_listed;

template<typename T>
struct is_one_of_listed<T>
{
    static constexpr bool value = false;
};

template<typename T, typename F, typename... Types>
struct is_one_of_listed<T, F, Types...>
{
    static constexpr bool value = std::is_same<T, F>::value || is_one_of_listed<T, Types...>::value;
};

template<typename T, typename... Types>
constexpr bool is_one_of_listed_v = is_one_of_listed<T, Types...>::value;
