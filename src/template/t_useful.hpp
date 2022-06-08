#pragma once

#include <type_traits>

struct none
{
    none()
    {}

    none(int)
    {}
};

// may be sometimes useful for SFINAE as rank<N> always more prefferable than rank<N - 1>
// https://stackoverflow.com/questions/30766965/is-there-a-generic-way-to-negate-a-decltype-condition-with-sfinae/30767152#30767152
template<unsigned N>
struct rank_p: rank_p<N - 1>
{
    enum valueType
    {
        value
    };
};

template<>
struct rank_p<0>
{
    enum valueType
    {
        value
    };
};


namespace cmt // compatibility with different standards
{

    #if __cplusplus < 201703L

        template<typename...>
        using void_t = void;

    #else

        using std::void_t;

    #endif

}


namespace
{

    struct _Wrapper_Def_Tag
    {};


    template<bool get, typename T>
    struct _GetTypeTypedefIf;

    template<typename T>
    struct _GetTypeTypedefIf<true, T>
    {
        typedef typename T::type type;
    };

    template<typename T>
    struct _GetTypeTypedefIf<false, T>
    {
        typedef T type;
    };


    template<typename TTrait, typename Enable = void>
    struct _GetMemberTypedefedType
    {
        static constexpr bool value = false;
    };

    template<typename TTrait>
    struct _GetMemberTypedefedType<TTrait, cmt::void_t<typename TTrait::type>>
    {
        static constexpr bool value = true;

        typedef typename TTrait::type type;
    };

}

/*
 * Used to differenciate between explicitly provided types and default ones in template arguments
 * Usage: template<typename T = TypeWrapperDefault<DefaultType>>
 */
template<typename T>
struct DefaultTypeWrapper: _Wrapper_Def_Tag
{
    typedef T type;
};

template<typename W>
constexpr bool is_wrapped_v = std::is_base_of<_Wrapper_Def_Tag, W>::value;

template<typename W>
using get_wrapped_t = typename _GetTypeTypedefIf<is_wrapped_v<W>, W>::type;


/*
 * FirstTypeIf:
 *  if enable is true, then
 *      FirstType::type = TypeTraitN::type,
 *          where TypeTraitN is first struct in TypeTraitsToTry list which has type typedef
 *      If no struct in TypeTraitsToTry list has type typedef, then this struct is empty
 *  if enable is false, then this struct is empty
 */
template<bool enable, typename... TypeTraitsToTry>
struct FirstTypeIf
{};

template<typename TFirst, typename... TOthers>
struct FirstTypeIf<true, TFirst, TOthers...>:
        std::conditional_t<_GetMemberTypedefedType<TFirst>::value,
            _GetMemberTypedefedType<TFirst>,
            FirstTypeIf<!_GetMemberTypedefedType<TFirst>::value, TOthers...>>
{};


// FirstType<TypeTraitsToTry...> equivalent to FirstTypeIf<true, TypeTraitsToTry...>
template<typename... TypeTraitsToTry>
using FirstType = FirstTypeIf<true, TypeTraitsToTry...>;


// Helper struct for FirstTypeIf trait
template<bool enable, typename T>
struct TypeIf
{};

template<typename T>
struct TypeIf<true, T>
{
    typedef T type;
};

// Helper struct for FirstTypeIf trait
template<typename T>
struct ReturnType
{
    typedef T type;
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
