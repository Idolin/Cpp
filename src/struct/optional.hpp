#pragma once

#include "../debug/def_debug.h"

struct none
{
    none()
    {}

    none(int)
    {}
};

template<typename T>
struct optional
{
    union
    {
        T value;
        bool nothing;
    };
    bool is_set;

    optional(): nothing(true), is_set(false)
    {}

    optional(const T& value): value(value), is_set(true)
    {}

    optional(T&& value): value(value), is_set(true)
    {}

    operator bool()
    {
        return is_set;
    }

    T& operator*()
    {
        ASSERT(is_set);
        return value;
    }

};

template<>
struct optional<none>
{
    bool is_set;

    optional(): is_set(false)
    {}

    optional(const none& value): is_set(true)
    {}

    optional(none&& value): is_set(true)
    {}

    operator bool()
    {
        return is_set;
    }
};