#pragma once

#include "./hash.hpp"

template<class S>
class singleton
{
    S s;

    singleton &operator=(singleton const &) = delete;

    singleton(singleton const &) = delete;

public:
    template<typename... Args>
    static S& instance(Args... args)
    {
        static singleton s(args...);
        return s.s;
    }

private:
    template<typename... Args>
    singleton(Args... args): s(args...)
    {}

    ~singleton()
    {}
};

constexpr uint64_t operator "" _gsh(const char *s, size_t size)
{
    return get_hash(s);
}

namespace g_static
{

    template<typename S, uint64_t namespace_gsh, uint64_t name_gsh, typename... Args>
    S& global_static_var(Args... args)
    {
        static S s(args...);
        return s;
    }

#define STATIC_VAR_CONSTRUCTOR(S, NAME, ...) \
    S& NAME() \
    { \
        static S s(__VA_ARGS__); \
        return s; \
    }

}
