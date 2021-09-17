#pragma once

#include "../template/type_tags.hpp"

#include <type_traits>
#include <utility>

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

        bool operator!=(const It& otr)
        {
            return !((*static_cast<It*>(this)).Impl::operator==(otr));
        }
    };

    template<typename Impl, typename It>
    struct _notEqualDefault<Impl, It, typename std::enable_if_t<has_inequality_operator_v<Impl>>>
    {
        static_assert(has_equality_operator_v<Impl>, "Iterator must have bool operator==");
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

        void _postIncrement()
        {
            (*static_cast<It*>(this)).Impl::operator++();
        }
    };

    template<typename Impl, typename It>
    struct _postIncrementDefault<Impl, It, typename std::enable_if_t<has_increment_method_v<Impl> && !has_post_increment_operator_v<Impl>>>
    {
        void _postIncrement()
        {
            (*static_cast<It*>(this)).Impl::increment();
        }
    };

    template<typename Impl, typename It>
    struct _postIncrementDefault<Impl, It, typename std::enable_if_t<has_post_increment_operator_v<Impl>>>
    {
        static_assert(!has_post_increment_operator_v<Impl> || !has_increment_method_v<Impl>,
            "Iterator must have either post-increment operator or increment method, not both");

        void _postIncrement()
        {
            (*static_cast<It*>(this)).Impl::operator++(0);
        }
    };


    template<typename Impl, typename It, bool has_pre_increment>
    struct _preIncrementDefault;

    template<typename Impl, typename It>
    struct _preIncrementDefault<Impl, It, false>
    {
        static_assert(!has_pre_increment_operator_v<Impl> || !has_increment_method_v<Impl>,
            "Iterator must have either pre-increment operator or increment method");

        void _preIncrement()
        {
            (*static_cast<It*>(this)).Impl::increment();
        }
    };

    template<typename Impl, typename It>
    struct _preIncrementDefault<Impl, It, true>
    {
        static_assert(!has_pre_increment_operator_v<Impl> || !has_increment_method_v<Impl>,
            "Iterator must have either pre-increment operator or increment method, not both");

        void _preIncrement()
        {
           (*static_cast<It*>(this)).Impl::operator++();
        }
    };


    template<typename Impl, typename It>
    struct _incrementDefault: _preIncrementDefault<Impl, It, has_pre_increment_operator_v<Impl>>,
            _postIncrementDefault<Impl, It>
    {};


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
