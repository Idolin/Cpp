#pragma once

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