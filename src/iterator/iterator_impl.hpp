#pragma once

#include "../template/type_tags.hpp"

#include <type_traits>
#include <utility>

#include <iostream>

namespace iterator_impl_def
{
    namespace
    {
        template<typename It>
        struct get_def_value
        {
            static_assert(is_dereferencable_v<It>, "Iterator must be dereferencable");

            typedef std::remove_reference_t<decltype(*std::declval<It>())> type;
        };

        template<typename It>
        struct get_def_reference
        {
            static_assert(is_dereferencable_v<It>, "Iterator must be dereferencable");

            typedef decltype(*std::declval<It>()) type;
        };

        template<typename It, typename Enable = void>
        struct get_def_pointer
        {
            static_assert(is_dereferencable_v<It>, "Iterator must be dereferencable");

            typedef std::add_pointer_t<std::remove_reference_t<decltype(*std::declval<It>())>> type;
        };

        template<typename It>
        struct get_def_pointer<It, typename std::enable_if_t<has_arrow_operator_v<It>>>
        {
            static_assert(is_dereferencable_v<It>, "Iterator must be dereferencable");

            typedef decltype(std::declval<It>().operator->()) type;
        };
    }

    template<typename It>
    using get_def_value_t = typename get_def_value<It>::type;

    template<typename It>
    using get_def_reference_t = typename get_def_reference<It>::type;

    template<typename It>
    using get_def_pointer_t = typename get_def_pointer<It>::type;


    template<typename Impl, typename It, typename Enable = void>
    struct _notEqualDefault
    {
        static_assert(has_equality_operator_v<Impl>, "Iterator must have bool operator==");
        static_assert(has_equality_operator_v<const Impl>, "Iterator's operator== must be const");

        bool operator!=(const It& otr) const
        {
            return !((*static_cast<const It*>(this)).Impl::operator==(otr));
        }
    };

    template<typename Impl, typename It>
    struct _notEqualDefault<Impl, It, typename std::enable_if_t<has_inequality_operator_v<Impl>>>
    {
        static_assert(has_equality_operator_v<Impl>, "Iterator must have bool operator==");
        static_assert(has_equality_operator_v<const Impl>, "Iterator's operator== must be const");
    };


    template<typename Impl, typename It, typename R, typename Enable = void>
    struct _arrowDefault
    {
        static_assert(is_dereferencable_v<R>, "Iterator pointer type better be dereferencable");

        R operator->()
        {
            return &((*static_cast<It*>(this)).Impl::operator*());
        }
    };

    template<typename Impl, typename It, typename R>
    struct _arrowDefault<Impl, It, R, typename std::enable_if_t<has_arrow_operator_v<Impl>>>
    {
        static_assert(is_dereferencable_v<R>, "Iterator pointer type better be dereferencable");
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _postIncrementDefault
    {
        static_assert(!has_pre_increment_operator_v<Impl> || !has_increment_method_v<Impl>,
            "Iterator must have either pre-increment operator or increment method");

        It _postIncrement()
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::operator++();
            return copy;
        }
    };

    template<typename Impl, typename It>
    struct _postIncrementDefault<Impl, It, typename std::enable_if_t<has_increment_method_v<Impl> && !has_post_increment_operator_v<Impl>>>
    {
        It _postIncrement()
        {
            It copy = *static_cast<It*>(this);
            (*static_cast<It*>(this)).Impl::increment();
            return copy;
        }
    };

    template<typename Impl, typename It>
    struct _postIncrementDefault<Impl, It, typename std::enable_if_t<has_post_increment_operator_v<Impl>>>
    {
    private:
        // Impl::operator++(int) return type
        typedef decltype(std::declval<Impl>()++) post_increment_t;

        // type iterator helper(It) will return for operator++(int): either It or Impl::operator++(int) return type
        typedef std::conditional_t<
            is_same_omit_cv_v<post_increment_t, Impl>,
                copy_cv_refernce_t<Impl, It>, post_increment_t> post_increment_ret;

    public:
        static_assert(!has_post_increment_operator_v<Impl> || !has_increment_method_v<Impl>,
            "Iterator must have either post-increment operator or increment method, not both");
        static_assert(is_dereferencable_v<post_increment_t>,
            "Iterator post-increment operator must return dereferencable object");

        post_increment_ret _postIncrement()
        {
            return (*static_cast<It*>(this)).Impl::operator++(0);
        }
    };


    template<typename Impl, typename It, typename Enable = void>
    struct _preIncrementDefault
    {
        static_assert(!has_pre_increment_operator_v<Impl> || !has_increment_method_v<Impl>,
            "Iterator must have either pre-increment operator or increment method");

        void _preIncrement()
        {
            (*static_cast<It*>(this)).Impl::increment();
        }
    };

    template<typename Impl, typename It>
    struct _preIncrementDefault<Impl, It, typename std::enable_if_t<has_pre_increment_operator_v<Impl>>>
    {
        static_assert(!has_pre_increment_operator_v<Impl> || !has_increment_method_v<Impl>,
            "Iterator must have either pre-increment operator or increment method, not both");

        void _preIncrement()
        {
           (*static_cast<It*>(this)).Impl::operator++();
        }
    };


    template<typename It, typename R, typename Enable = void>
    struct _postDecrementDefault
    {
        static_assert(has_pre_decrement_operator_v<It>, "Iterator must have pre-decrement operator");

        R operator--(int)
        {
            R copy = *static_cast<R*>(this);
            static_cast<It*>(this)->decrement();
            return copy;
        }
    };

    template<typename It, typename R>
    struct _postDecrementDefault<It, R, typename std::enable_if_t<has_post_decrement_operator_v<It>>>
    {
        static_assert(has_pre_decrement_operator_v<It>, "Iterator must have pre-decrement operator");
    };
}
