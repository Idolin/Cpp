#pragma once

#include <iterator>
#include <type_traits>

#include "iterator_check.hpp"


namespace
{

    template<typename IT, typename Enable = void>
    struct IteratorTypeOrVoid
    {
        typedef void iterator_type;
    };

    template<typename IT>
    struct IteratorTypeOrVoid<IT, std::enable_if_t<is_valid_stl_bidirectional_iterator_v<typename IT::iterator_type>>>
    {
        typedef typename IT::iterator_type iterator_type;
    };

    template<typename IT, typename Enable = void>
    struct ReverseIteratorTypeOrVoid
    {
        typedef void reverse_iterator;
    };

    template<typename IT>
    struct ReverseIteratorTypeOrVoid<IT, std::enable_if_t<is_valid_stl_bidirectional_iterator_v<IT>>>
    {
        typedef std::reverse_iterator<IT> reverse_iterator;
    };

    template<typename IT, typename Enable = void>
    struct ReReverseIteratorTypeOrVoid
    {
        typedef void reverse_iterator;
    };

    template<typename IT>
    struct ReReverseIteratorTypeOrVoid<IT, std::enable_if_t<is_valid_stl_bidirectional_iterator_v<typename IT::iterator_type>>>
    {
        typedef std::reverse_iterator<typename IT::iterator_type> reverse_iterator;
    };

}


/*
 * If IT is not a valid iterator bidirectional than produces compile time error.
 * If IT is an std::reverse_iterator<ITB> than return ITB.
 * Otherwise returns std::reverse_iterator<IT>.
 */
template<typename IT>
using reverse_iterator = std::conditional_t<is_valid_stl_bidirectional_iterator_v<IT, true> &&
                            std::is_same<IT, typename ReReverseIteratorTypeOrVoid<IT>::reverse_iterator>::value,
    typename IteratorTypeOrVoid<IT>::iterator_type,
    typename ReverseIteratorTypeOrVoid<IT>::reverse_iterator>;
