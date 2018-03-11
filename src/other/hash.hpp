#pragma once

#include "../debug/def_debug.h"

#include <stdint.h>

struct Hashable
{
    virtual uint64_t hash() const = 0;
};

template<class Self, bool lazy = true>
class HashableStored : Hashable
{};

template<class Self>
class HashableStored<Self, true>: Hashable
{
private:
    mutable uint64_t hash_value;
protected:
    mutable bool changed;
public:
    HashableStored(uint64_t hash_value): hash_value(hash_value), changed(false)
    {}

    HashableStored(): changed(true)
    {}

private:
    bool equals(const Self &) const = 0;

protected:
    virtual uint64_t hash_recalc() const = 0;

public:
    uint64_t hash() const final
    {
        if(changed)
            hash_value = hash_recalc();
        return hash_value;
    }

    bool operator==(const Self &b) const final
    {
        return ((hash() == b.hash()) && this->equals(b));
    }

    bool operator!=(const Self &b) const final
    {
        return !(*this == b);
    }
};

template<class Self>
class HashableStored<Self, false>: Hashable
{
private:
    uint64_t hash_value;
public:
    HashableStored(uint64_t hash_value): hash_value(hash_value)
    {}

private:
    virtual bool equals(const Self &) const = 0;

protected:
    virtual uint64_t hash_recalc() const = 0;

public:
    uint64_t hash() const final
    {
        return hash_value;
    }

    bool operator==(const Self &b) const final
    {
        return ((hash() == b.hash()) && this->equals(b));
    }

    bool operator!=(const Self &b) const final
    {
        return !(*this == b);
    }
};
