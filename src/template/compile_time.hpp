#pragma once

#include <array>
#include <type_traits>

#include "commonmethods.hpp"

template<typename T, typename SizeType, size_t... i>
static constexpr auto _make_array(T(*func)(SizeType index), std::index_sequence<i...>)
{
    return std::array<T, sizeof...(i)>{{func(i)...}};
}

template<typename T, size_t array_size, typename SizeType = unsigned,
    typename = typename std::enable_if_t<std::is_integral<SizeType>::value>>
constexpr std::array<T, array_size> get_compile_time_array(T(*func)(SizeType index))
{
    return _make_array<T, SizeType>(func, std::make_index_sequence<array_size>{});
}

template<typename T, size_t array_size, typename SizeType, T (*func)(SizeType),
    typename = typename std::enable_if_t<std::is_integral<SizeType>::value>>
constexpr std::array<T, array_size> compile_time_array = get_compile_time_array<T, array_size, SizeType>(func);
