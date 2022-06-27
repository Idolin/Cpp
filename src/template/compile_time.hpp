#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../other/defdef.h"



namespace
{

    template<typename AltStr, typename... Str>
    struct Alternate
    {
    private:
        using TypeTuple = std::tuple<Str...>;

    public:
        template<std::size_t Index>
        using get = std::conditional_t<Index % 2 == 0, AltStr, decltype(std::get<Index / 2>(std::declval<TypeTuple>))>;
    };


    template<typename SizeType, template<SizeType... Indexes> class C, SizeType N>
    struct InstantiateWithIntegerSequenceHelper
    {
    private:
        template<SizeType... Indexes>
        static constexpr auto dummy_func(std::integer_sequence<SizeType, Indexes...>) -> C<Indexes...>;

    public:
        using type = decltype(dummy_func(std::make_integer_sequence<SizeType, N>{}));
    };

}


template<typename SizeType, template<SizeType... Indexes> class C, SizeType N>
using InstantiateWithIntegerSequence = typename InstantiateWithIntegerSequenceHelper<SizeType, C, N>::type;

template<template<std::size_t... Indexes> class C, std::size_t N>
using InstantiateWithIndexSequence = typename InstantiateWithIntegerSequenceHelper<std::size_t, C, N>::type;


#if __cplusplus < 201703L

    // sadly we can't use std::array instead here as non-const std::array::operator[] is constexpr only since C++17
    template<std::size_t Size>
    struct CharArray
    {
        char value[Size];

        constexpr char& operator[](std::size_t index)
        {
            return value[index];
        }

        constexpr const char& operator[](std::size_t index) const
        {
            return value[index];
        }

        constexpr std::size_t size() const
        {
            return Size;
        }
    };

#else

    template<std::size_t Size>
    using CharArray = std::array<char, Size>;

#endif

// if we need to define value we have to do it out-of-class, because inside class we can only declare it
// struct .. { constexpr value = ...; } is still only a declaration, so taking address of it(making reference to it) is UB
// see https://stackoverflow.com/questions/47886481/static-constexpr-template-member-gives-undefined-reference-when-specialized
template<typename Str>
struct StrDefined
{
private:
    template<std::size_t... Indexes>
    struct StrDefinedImpl
    {
        static const char value[sizeof...(Indexes)];
    };

public:
    static const char (&value)[sizeof(Str::value)];
};

template<typename Str>
template<std::size_t... Indexes>
const char StrDefined<Str>::StrDefinedImpl<Indexes...>::value[sizeof...(Indexes)] = {Str::value[Indexes]...};

template<typename Str>
const char (&StrDefined<Str>::value)[sizeof(Str::value)] = InstantiateWithIndexSequence<StrDefined<Str>::StrDefinedImpl, sizeof(Str::value)>::value;


// heavily inspired by answer to this question:
// https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
// parameter pack expansion:
// https://stackoverflow.com/questions/25680461/variadic-template-pack-expansion
template<typename... Str>
struct Concat
{
private:
    template<typename SizeType = std::size_t>
    static constexpr std::size_t sum(SizeType arg = 0)
    {
        return arg;
    }

    template<typename FirstSizeType, typename... SizeTypes>
    static constexpr std::size_t sum(FirstSizeType arg, SizeTypes... args)
    {
        return arg + sum(args...);
    }

    template<std::size_t N>
    static constexpr void append(CharArray<N> &result, const char *s, std::size_t &index)
    {
        for(std::size_t pos = 0;s[pos];pos++)
            result[index++] = s[pos];
    }

    template<std::size_t N, std::size_t M>
    static constexpr void append(CharArray<N> &result, const CharArray<M> &s, std::size_t &index)
    {
        for(std::size_t pos = 0;s[pos];pos++)
            result[index++] = s[pos];
    }

    template<typename... BoolTypes>
    static constexpr bool check_all(BoolTypes... bools)
    {
        bool check = true;
        using expander = int[];
        // array length must be greater than zero (it may be zero if created by new[] operator)
        MAYBE_UNUSED expander{0, (check &= bools, 0)...};
        return check;
    }

    static constexpr auto concatenate() noexcept
    {
        constexpr bool check_len = check_all((sizeof(Str::value) > 0)...);
        static_assert(check_len, "All strings(Str...) lengths must be greater then zero to accomodate end-of-line symbol('\\0')");

        // we require all Str to end with end-of-line symbol ('\0')
        //  but it's accepted if it also appears somewhere before end
        constexpr bool check_eof = check_all((Str::value[sizeof(Str::value) - 1] == '\0')...);
        static_assert(check_eof, "All strings(Str...) values must end with end-of-line symbol('\\0')");

        auto result = CharArray<sum(sizeof(Str::value)...) - sizeof...(Str) + 1>{};

        std::size_t index = 0;
        using expander = int[];
        // array length must be greater than zero (it may be zero if created by new[] operator)
        MAYBE_UNUSED expander{0, (append(result, Str::value, index), 0)...};

        result[index] = '\0';

        return result;
    }

public:
    static constexpr auto value = concatenate();
};


struct CommaDelimiter
{
    static constexpr const char value[] = ", ";
};

template<typename DelimiterStr, typename... Str>
struct Join;

template<typename DelimiterStr>
struct Join<DelimiterStr>
{
    static constexpr const char value[] = "";
};

template<typename DelimiterStr, typename FirstStr, typename... Str>
struct Join<DelimiterStr, FirstStr, Str...>
{
private:
    template<std::size_t... Indexes>
    struct JoinImpl
    {
        static constexpr auto value = Concat<FirstStr, typename Alternate<DelimiterStr, Str...>::template get<Indexes>...>::value;
    };

public:
    static constexpr auto value = InstantiateWithIndexSequence<JoinImpl, sizeof...(Str) * 2>::value;
};


struct Stringify
{
    template<typename IntegralType, typename Enable = std::enable_if_t<std::is_integral<IntegralType>::value>>
    constexpr Stringify(IntegralType number): value{}
    {
        using DivisorType = std::conditional_t<std::is_unsigned<IntegralType>::value, unsigned, int>;
        DivisorType divisor = 10;
        std::size_t index = 0;
        if(number == 0)
            value[index++] = '0';
        else
        {
            if(number < 0)
                divisor = -10;
            while(number != 0)
            {
                value[index++] = '0' + (number % divisor);
                number /= divisor;
            }
            if(divisor < 0) // because initial number was negative
                value[index++] = '-';
            // symbols in reverse order now, so we need to reverse it back to normal order
            // we can't use std::swap here as it's declared constexpr only since C++20
            for(std::size_t i = 0;i < index / 2;i++)
            {
                char tmp = value[i];
                value[i] = value[index - i - 1];
                value[index - i - 1] = tmp;
            }
        }
        value[index] = '\0';
    }

    // we can't really use std::numeric_limits there as it's not guaranteed to exist for
    //  std::intmax_t/std::uintmax_t (as it may be implementation-defined type)
    CharArray<std::max(sizeof(std::intmax_t) + 1u, sizeof(std::uintmax_t)) + 1u> value;

private:
    static constexpr std::size_t get_digits10(std::size_t bits)
    {
        // 28/93 (~= 0.301075) is a rational approximation of log10(2) (~= 0.301030)
        return (bits * 28u) / 93u + 1u;
    }

public:

    template<std::intmax_t N>
    struct FromSigned;

    template<std::uintmax_t N>
    struct FromUnsigned;
};

template<std::intmax_t N>
struct Stringify::FromSigned
{
    static constexpr auto value = Stringify(N).value;
};

template<std::uintmax_t N>
struct Stringify::FromUnsigned
{
    static constexpr auto value = Stringify(N).value;
};


template<typename T, std::size_t... i>
static constexpr auto _make_array(std::function<T(std::size_t index)> func, std::index_sequence<i...>)
{
    return std::array<T, sizeof...(i)>{{func(i)...}};
}

template<typename T, std::size_t array_size>
constexpr std::array<T, array_size> get_compile_time_array(std::function<T(std::size_t index)> func)
{
    return _make_array<T>(func, std::make_index_sequence<array_size>{});
}

template<typename T, std::size_t array_size, T(*func)(std::size_t index)>
constexpr std::array<T, array_size> compile_time_array = get_compile_time_array<T, array_size>(func);
