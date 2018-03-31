#pragma once

template<unsigned N>
struct _rank: _rank<N - 1>
{};

template<>
struct _rank<0>
{};

