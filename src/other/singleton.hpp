#pragma once

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

namespace g_static
{

    const int default_unit_id = 0;
    const int test_global_static_id = 3;

    template<class S, int id = default_unit_id, typename... Args>
    S& global_static_var(Args... args)
    {
        static S s(args...);
        return s;
    }

#define GLOBAL_STATIC_VAR_CONSTRUCTOR(S, id, ...) \
    template<> \
    S& global_static_var<S, id>() \
    { \
        static S s(__VA_ARGS__); \
        return s; \
    }

#define STATIC_VAR_CONSTRUCTOR(S, NAME, ...) \
    S& NAME() \
    { \
        static S s(__VA_ARGS__); \
        return s; \
    }

}