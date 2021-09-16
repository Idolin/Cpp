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
            typedef std::remove_reference_t<decltype(*std::declval<It>())> type;
        };

        template<typename It>
        struct get_def_reference
        {
            typedef decltype(*std::declval<It>()) type;
        };

        template<typename It, typename Enable = void>
        struct get_def_pointer
        {
            typedef std::add_pointer_t<std::remove_reference_t<decltype(*std::declval<It>())>> type;
        };

        template<typename It>
        struct get_def_pointer<It, typename std::enable_if_t<has_arrow_operator_v<It>>>
        {
            typedef decltype(std::declval<It>().operator->()) type;
        };
    }

    template<typename It>
    using get_def_value_t = typename get_def_value<It>::type;

    template<typename It>
    using get_def_reference_t = typename get_def_reference<It>::type;

    template<typename It>
    using get_def_pointer_t = typename get_def_pointer<It>::type;


    template<typename It, typename Enable = void>
    struct _notEqualDefault
    {
        static_assert(has_equality_operator_v<It>, "Iterator must have bool operator==");

        bool operator!=(const It& otr)
        {
            return !(*static_cast<It*>(this) == otr);
        }
    };

    template<typename It>
    struct _notEqualDefault<It, typename std::enable_if_t<has_inequality_operator_v<It>>>
    {
        static_assert(has_equality_operator_v<It>, "Iterator must have bool operator==");
    };


    template<typename It, typename R, typename Enable = void>
    struct _arrowDefault
    {
        static_assert(is_dereferencable_v<R>, "Iterator pointer type better be dereferencable");

        R operator->()
        {
            return &(static_cast<It*>(this)->operator*());
        }
    };

    template<typename It, typename R>
    struct _arrowDefault<It, R, typename std::enable_if_t<has_arrow_operator_v<It>>>
    {
        static_assert(is_dereferencable_v<R>, "Iterator pointer type better be dereferencable");
    };


    template<typename It, typename Enable = void>
    struct _postIncrementDefault
    {
        static_assert(has_pre_increment_operator_v<It>, "Iterator must have pre-increment operator");

        It operator++(int)
        {
            It copy = *static_cast<It*>(this);
            ++*static_cast<It*>(this);
            return copy;
        }
    };

    template<typename It>
    struct _postIncrementDefault<It, typename std::enable_if_t<has_inequality_operator_v<It>>>
    {
        static_assert(has_pre_increment_operator_v<It>, "Iterator must have pre-increment operator");
    };
}
