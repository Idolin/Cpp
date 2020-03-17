#pragma once

#include "../debug/def_debug.h"
#include "../template/type_tags.hpp"
#include "../template/typemethods.hpp"
#include "../template/valuemethods.hpp"

#include <functional>
#include <stdint.h>
#include <type_traits>

struct AnyHashable
{};

struct NoHashable: AnyHashable
{
    bool hash_equals(const AnyHashable &b) const
    {
        return false;
    }
};

struct Hashable: AnyHashable
{
    virtual uint64_t hash() const noexcept = 0;

    virtual bool hash_equals(const Hashable &b) const final
    {
        return (hash() == b.hash());
    }
};

namespace std
{
    template <>
    struct hash<Hashable>
    {
        size_t operator()(const Hashable &s) const noexcept
        {
            return static_cast<size_t>(s.hash());
        }
    };
}

template<bool enabled=true>
using HashableConditional = std::conditional<enabled, Hashable, NoHashable>;

template<bool lazy = true>
struct HashableStored: Hashable
{};

template<bool lazy=true>
struct NoHashableStrored: protected AnyHashable
{
    bool is_changed() const
    {
        return false;
    }

 protected:
     void set_hash(uint64_t value) const
     {}

     uint64_t get_hash_value() const
     {
         return 0;
     }
};

template<>
struct HashableStored<true>: Hashable
{
private:
    mutable uint64_t hash_value;
    mutable bool changed;
public:
    HashableStored(uint64_t hash_value): hash_value(hash_value), changed(false)
    {}

    HashableStored(): changed(true)
    {}

    HashableStored(const HashableStored& otr): hash_value(otr.hash_value), changed(otr.changed)
    {}

protected:
    virtual uint64_t hash_recalc() const = 0;

    void set_hash(uint64_t value) const
    {
        hash_value = value, changed = false;
    }

    uint64_t get_hash_value() const
    {
        return hash_value;
    }

    void hash_changed() const
    {
        changed = true;
    }

public:
    uint64_t hash() const noexcept final
    {
        if(changed)
            hash_value = hash_recalc(), changed = false;
        return hash_value;
    }

    bool is_changed() const
    {
        return changed;
    }
};

template<>
struct HashableStored<false>: Hashable
{
private:
    mutable uint64_t hash_value;
public:
    HashableStored(uint64_t hash_value = 0): hash_value(hash_value)
    {}

protected:
    void set_hash(uint64_t value) const
    {
        hash_value = value;
    }

    uint64_t get_hash_value() const
    {
        return hash_value;
    }

public:
    uint64_t hash() const noexcept final
    {
        return hash_value;
    }
};

template<bool lazy=true, bool enabled=true>
using HashableStoredConditional = typename std::conditional<enabled, HashableStored<lazy>, NoHashableStrored<lazy>>::type;

template<typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_pointer<T>::value, uint64_t>::type get_hash(T x)
{
    return static_cast<uint64_t>(_valueMethods<T>::to_unsigned(x));
}

template<typename T>
typename std::enable_if<std::is_base_of<Hashable, T>::value, uint64_t>::type get_hash(const T &x)
{
    return x.hash();
}

template<typename T>//useful only for templates, not expected to be called from runtime
typename std::enable_if<!is_hashable<T>::value, uint64_t>::type get_hash(const T &x)
{
    ASSERT(false);
    return 0;
}
