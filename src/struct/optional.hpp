#pragma once

#include "../debug/def_debug.h"
#include "../template/t_useful.hpp"
#include "../template/typemethods.hpp"

template<typename T>
struct optional
{
private:
    union
    {
        T value;
        bool nothing;
    };
    bool is_set;

public:
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

template<typename T, bool store>
struct ConditionalStored;

template<typename T>
struct ConditionalStored<T, true>
{
private:
    T value;

public:
    ConditionalStored()
    {}

    ConditionalStored(typename def_get_by<T>::type x): value(x)
    {}

    typename def_get_by<T>::type operator=(const ConditionalStored<T, true> &otr)
    {
        value = otr.value;
    }

    typename def_get_by<T>::type operator=(ConditionalStored<T, true> &&otr)
    {
        value = otr.value;
    }

    typename def_get_by<T>::type get_value() const
    {
        return value;
    }

    void set_value(typename def_get_by<T>::type x)
    {
        value = x;
    }
};

template<typename T>
struct ConditionalStored<T, false>
{
public:
    ConditionalStored()
    {}

    ConditionalStored(typename def_get_by<T>::type x)
    {}

    typename def_get_by<T>::type operator=(const ConditionalStored<T, true> &otr)
    {}

    typename def_get_by<T>::type operator=(ConditionalStored<T, true> &&otr)
    {}

    typename def_get_by<T>::type get_value() const //useful only for templates, not expected to be called in runtime
    {
        ASSERT(false);
        return (T)(0);
    }

    void set_value(typename def_get_by<T>::type x)
    {}
};
