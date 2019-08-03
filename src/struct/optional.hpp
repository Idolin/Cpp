#pragma once

#include "../debug/def_debug.h"
#include "../template/t_useful.hpp"
#include "../template/typemethods.hpp"
#include "../other/serializable.hpp"

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

    serialized serialize() const
    {
        if(!is_set)
            return serialized(new unsigned char[1]{0}, 1);
        serialized s = ::serialize(value);
        unsigned char *buffer = new unsigned char[s.size() + 1];
        buffer[0] = 1;
        _copy(s.data(), s.size(), buffer + 1);
        return serialized(buffer, s.size() + 1);
    }

    static optional deserialize(unsigned char *buffer, size_t buffer_length)
    {
        if(buffer[0])
            return optional(::deserialize<T>(buffer + 1, buffer_length - 1));
        else
            return optional();
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
