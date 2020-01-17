#pragma once

#include <array>
#include <functional>
#include <type_traits>

#include "commonmethods.hpp"

template<typename T, size_t... i>
static constexpr auto _make_array(std::function<T(std::size_t index)> func, std::index_sequence<i...>)
{
    return std::array<T, sizeof...(i)>{{func(i)...}};
}

template<typename T, size_t array_size>
constexpr std::array<T, array_size> get_compile_time_array(std::function<T(std::size_t index)> func)
{
    return _make_array<T>(func, std::make_index_sequence<array_size>{});
}

template<typename T, size_t array_size, T(*func)(std::size_t index)>
constexpr std::array<T, array_size> compile_time_array = get_compile_time_array<T, array_size>(func);
