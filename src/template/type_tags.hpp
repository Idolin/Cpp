#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

template<typename T>
struct is_numeric
{
    enum {
        value = (std::is_integral<T>::value && !std::is_same<std::remove_cv_t<T>, bool>::value) ||
                std::is_floating_point<T>::value
    };
};

template<typename T>
struct is_cstr
{
    enum {
        value = std::is_pointer<T>::value && 
            (std::is_same<
                typename std::remove_cv_t<
                    typename std::remove_reference<typename std::remove_extent<T>::type>::type>, char>::value || 
            std::is_same<typename std::remove_cv_t<typename std::remove_pointer<T>::type>, char>::value)
    };
};

template<typename T = void>
struct is_bit_movable;

template<>
struct is_bit_movable<void>
{};

template<typename T>
struct is_bit_movable
{
    enum {
        value = std::is_pod<T>::value || std::is_pointer<T>::value ||
                std::is_base_of<is_bit_movable<>, T>::value
    };
};

template<typename T = void>
struct is_bit_copyable;

template<>
struct is_bit_copyable<void>
{};

template<typename T>
struct is_bit_copyable
{
    enum
    {
        value = std::is_pod<T>::value || std::is_pointer<T>::value ||
                std::is_base_of<is_bit_copyable<>, T>::value
    };
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
    enum
    {
        value = std::is_base_of<mergeable<T>, T>::value
    };
};


#define IMPL_TYPE_TAGS_STRUCT_CHECK_T(name, ...) \
    template<typename T> \
    struct name<T, __VA_ARGS__> \
    { \
        enum \
        { \
            value = true \
        }; \
    };

#define IMPL_TYPE_TAGS_STRUCT_CHECK(name, ...) \
    template<typename T, typename Enable = void> \
    struct name \
    { \
        enum \
        { \
            value = false \
        }; \
    }; \
    \
    IMPL_TYPE_TAGS_STRUCT_CHECK_T(name, ## __VA_ARGS__) \
    \
    template<typename T> \
    constexpr bool name ## _v = name<T>::value;


#define IMPL_TYPE_TAGS_STRUCT_T_IS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK_T(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_STRUCT_IS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)

#define IMPL_TYPE_TAGS_STRUCT_T_HAS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK_T(has_ ## name, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_STRUCT_HAS(name, ...) IMPL_TYPE_TAGS_STRUCT_CHECK(has_ ## name, typename std::enable_if_t<__VA_ARGS__>)


IMPL_TYPE_TAGS_STRUCT_IS(less_comparable, std::is_same<
        decltype(std::declval<typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>() <
         std::declval<typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>()), bool>::value)

IMPL_TYPE_TAGS_STRUCT_IS(dereferencable, std::is_lvalue_reference<
        decltype(*std::declval<typename std::add_lvalue_reference<T>::type>())>::value)

IMPL_TYPE_TAGS_STRUCT_HAS(arrow_operator, std::is_pointer<decltype(std::declval<T>().operator->())>::value)

// using is_constructible instead of is_convertible to allow operator== return type contextually convertible to bool as well
IMPL_TYPE_TAGS_STRUCT_HAS(equality_operator, std::is_constructible<bool, decltype(std::declval<T>() == std::declval<T>())>::value)

// using is_constructible instead of is_convertible to allow operator!= return type contextually convertible to bool as well
IMPL_TYPE_TAGS_STRUCT_HAS(inequality_operator, std::is_constructible<bool, decltype(std::declval<T>() != std::declval<T>())>::value)

IMPL_TYPE_TAGS_STRUCT_HAS(pre_increment_operator,
        std::is_same<decltype(++std::declval<T>()), std::add_lvalue_reference_t<T>>::value)

IMPL_TYPE_TAGS_STRUCT_CHECK(has_post_increment_operator,
        decltype(std::declval<T>()++, void()))

IMPL_TYPE_TAGS_STRUCT_HAS(increment_method,
        std::is_same<decltype(std::declval<T>().increment()), void>::value)

IMPL_TYPE_TAGS_STRUCT_HAS(pre_decrement_operator,
        std::is_same<decltype(--std::declval<T>()), std::add_lvalue_reference_t<T>>::value)

IMPL_TYPE_TAGS_STRUCT_CHECK(has_post_decrement_operator,
        decltype(std::declval<T>()--, void()))

IMPL_TYPE_TAGS_STRUCT_HAS(decrement_method,
        std::is_same<decltype(std::declval<T>().decrement()), void>::value)

IMPL_TYPE_TAGS_STRUCT_IS(hashable, std::is_integral<T>::value || std::is_pointer<T>::value || is_cstr<T>::value)

IMPL_TYPE_TAGS_STRUCT_T_IS(hashable, std::is_same<
        decltype(std::declval<typename std::add_lvalue_reference<
                typename std::add_const<T>::type>::type>().hash()), uint64_t>::value)

IMPL_TYPE_TAGS_STRUCT_IS(block_splittable, std::is_integral<T>::value || std::is_pointer<T>::value)

IMPL_TYPE_TAGS_STRUCT_T_IS(block_splittable, std::is_same<decltype(std::declval<
             typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>().size()), std::size_t>::value &&
        is_block_splittable<decltype(std::declval<
                typename std::add_lvalue_reference<
                        typename std::add_const<T>::type>::type>().operator[][static_cast<std::size_t>(0)])>::value)

IMPL_TYPE_TAGS_STRUCT_IS(block_iterable, is_block_splittable<T>::value)

IMPL_TYPE_TAGS_STRUCT_IS(block_iterable_class, std::is_same<
        decltype(std::declval<typename std::add_lvalue_reference<typename std::add_const<T>::type>::type>().cbegin()),
        decltype(std::declval<typename std::add_lvalue_reference<
                typename std::add_const<T>::type>::type>().cend())>::value &&
    is_block_iterable<decltype(*(std::declval<typename std::add_lvalue_reference<
            typename std::add_const<T>::type>::type>().cbegin()))>::value)

IMPL_TYPE_TAGS_STRUCT_T_IS(block_iterable, is_block_iterable_class<T>::value)


#define IMPL_TYPE_TAGS_CHECK_TWO_T(name, ...) \
    template<typename T, typename U> \
    struct name<T, U, __VA_ARGS__> \
    { \
        enum \
        { \
            value = true \
        }; \
    };

#define IMPL_TYPE_TAGS_CHECK_TWO_(name, def_copy, ...) \
    template<typename T, typename U def_copy , typename Enable = void> \
    struct name \
    { \
        enum \
        { \
            value = false \
        }; \
    }; \
    \
    IMPL_TYPE_TAGS_CHECK_TWO_T(name, ## __VA_ARGS__) \
    \
    template<typename T, typename U def_copy > \
    constexpr bool name ## _v = name<T, U>::value;

#define IMPL_TYPE_TAGS_CHECK_TWO(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_(name, , ## __VA_ARGS__)

#define IMPL_TYPE_TAGS_CHECK_TWO_COPY(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_(name, = T, ## __VA_ARGS__)


#define IMPL_TYPE_TAGS_CHECK_TWO_T_IS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_T(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_IS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)
#define IMPL_TYPE_TAGS_CHECK_TWO_COPY_IS(name, ...) IMPL_TYPE_TAGS_CHECK_TWO_COPY(is_ ## name, typename std::enable_if_t<__VA_ARGS__>)


IMPL_TYPE_TAGS_CHECK_TWO_COPY(is_subtractable,
                              decltype(std::declval<T>() - std::declval<U>(), void()))
IMPL_TYPE_TAGS_CHECK_TWO_COPY(is_addable,
                              decltype(std::declval<T>() + std::declval<U>(), void()))
IMPL_TYPE_TAGS_CHECK_TWO_IS(explicitly_convertible,
                            std::is_constructible<U, T>::value && !std::is_convertible<T, U>::value)


template<typename T, typename U>
struct is_same_omit_cv
{
    enum
    {
        value = std::is_same<std::remove_cv_t<T>, std::remove_cv_t<U>>::value
    };
};

template<typename T, typename U>
constexpr bool is_same_omit_cv_v = is_same_omit_cv<T, U>::value;

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
    typedef set_cv_reference_t<T, std::is_const<F>::value,
        std::is_volatile<F>::value, std::is_lvalue_reference<F>::value, std::is_lvalue_reference<F>::value> type;
};

template<typename F, typename T>
using copy_cv_refernce_t = typename copy_cv_refernce<F, T>::type;
