#pragma once

#include <type_traits>
#include <utility>

#include "../template/compile_time.hpp"


namespace type_generator
{
    template<typename T>
    struct TypeDescription
    {
        static constexpr const char description[] = "undescribed type";

        using type = T;
    };

    template<typename T, typename Enable = void>
    struct TypeDescriptionStr
    {
    private:
        template<std::size_t... Indexes>
        struct TypeDescriptionStrImpl
        {
            static constexpr CharArray<sizeof...(Indexes)> value = {T::description[Indexes]...};
        };

    public:
        static constexpr auto value = InstantiateWithIndexSequence<TypeDescriptionStrImpl, sizeof(T::description)>::value;
    };

    template<typename T>
    struct TypeDescriptionStr<T, std::enable_if_t<std::is_same<std::remove_cv_t<decltype(T::value)>, CharArray<sizeof(T::value)>>::value>>
    {
        static constexpr auto value = T::value;
    };

    template<typename T, unsigned SequenceIndex>
    struct TypeDescriptionSequence: TypeDescription<T>
    {
    private:
        struct TMessagePrefix
        {
            static constexpr const char value[] = "type_generator::Sequence[index = ";
        };

        struct TMessageSuffix
        {
            static constexpr const char value[] = "]";
        };

    public:
        static constexpr auto description = Concat<TMessagePrefix, Stringify::FromUnsigned<SequenceIndex>, TMessageSuffix>::value;
    };


    template<typename... Ts>
    struct TypeTuple
    {
        template<template<typename...> class C, typename... Args>
        static C<Ts...> instantiate(Args&&... args)
        {
            return C<Ts...>(std::forward<Args>(args)...);
        }
    };

    namespace
    {

        template<std::size_t Index, typename... Ts>
        struct SequenceImpl;

        template<std::size_t Index, typename First, typename... Ts>
        struct SequenceImpl<Index, First, Ts...>
        {
            using CurrentTypes = TypeTuple<TypeDescriptionSequence<First, Index>>;
            using Next = SequenceImpl<Index + 1, Ts...>;
        };

        template<std::size_t Index>
        struct SequenceImpl<Index>
        {
            using CurrentTypes = void;
            using Next = void;
        };

    }


    template<typename... Ts>
    using Sequence = SequenceImpl<0, Ts...>;

}
