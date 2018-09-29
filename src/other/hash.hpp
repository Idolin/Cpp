#pragma once

#include "../debug/def_debug.h"

#include <stdint.h>

struct Hashable
{
    virtual uint64_t hash() const = 0;

    virtual bool hash_equals(const Hashable &b) const final
    {
        return (hash() == b.hash());
    }
};

template<bool lazy = true>
struct HashableStored: Hashable
{};

template<>
struct HashableStored<true>: Hashable
{
protected:
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

public:
    uint64_t hash() const final
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
protected:
    uint64_t hash_value;
public:
    HashableStored(uint64_t hash_value = 0): hash_value(hash_value)
    {}

public:
    uint64_t hash() const final
    {
        return hash_value;
    }
};
