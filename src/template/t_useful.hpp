#pragma once

#include <type_traits>

struct none
{
    none()
    {}

    none(int)
    {}
};

template<unsigned N>
struct _rank: _rank<N - 1>
{
    enum valueType
    {
        value
    };
};

template<>
struct _rank<0>
{
    enum valueType
    {
        value
    };
};

namespace t_adapter
{
    template<int n, typename... Types>
    struct get_type;

    template<int n, typename First, typename... Types>
    struct get_type<n, First, Types...>
    {
        typedef typename get_type<n - 1, Types...>::type type;
    };

    template<typename First, typename... Types>
    struct get_type<0, First, Types...>
    {
        typedef First type;
    };
    
    struct t_tuple_tag
    {};

    template<typename... Types>
    struct t_tuple: t_tuple_tag
    {
        // typedef typename get_type<0, Types...>::type T1;
        // typedef typename get_type<1, Types...>::type T2;
        // typedef typename get_type<2, Types...>::type T3;

        template<int n>
        struct type_n
        {
            typedef typename get_type<n, Types...>::type type;
        };
    };
    
    template<typename T, typename Enable = void>
    struct make_t_tuple;
    
    template<typename T>
    struct make_t_tuple<T, std::enable_if_t<std::is_base_of<t_tuple_tag, T>::value>>
    {
        typedef T type;
    };
    
    template<typename T>
    struct make_t_tuple<T, std::enable_if_t<!std::is_base_of<t_tuple_tag, T>::value>>
    {
        typedef t_tuple<T> type;
    };

    template<typename... Types>
    struct once
    {
        typedef t_tuple<Types...> TypeSeq;

        typedef void NextAdapter;
    };

    template<typename... Types>
    struct twice
    {
        typedef t_tuple<Types...> TypeSeq;

        typedef once<Types...> NextAdapter;
    };
    
    template<typename... Types>
    struct seq;
    
    template<typename First, typename... Types>
    struct seq<First, Types...>
    {
        typedef typename make_t_tuple<First>::type TypeSeq;
        
        typedef seq<Types...> NextAdapter;
    };
    
    template<typename First>
    struct seq<First>
    {
        typedef typename make_t_tuple<First>::type TypeSeq;
        
        typedef void NextAdapter;
    };
}
