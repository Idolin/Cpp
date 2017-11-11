#pragma once

#include "../other/hash.hpp"
#include "../debug/def_debug.h"

#include <math.h>
#include <memory>
#include <typeinfo>

using std::enable_if;
using std::is_base_of;
using std::make_shared;
using std::shared_ptr;

struct ExprTag : Hashable
{
    virtual ~ExprTag() = default;
};

template<class ETag = ExprTag>
struct Expression : ETag
{
    shared_ptr<ETag> expr;

    bool operator==(const Expression<ETag> &b) const
    {
        return *expr == b;
    }

    uint64_t hash() const
    {
        return expr->hash();
    }
};

template<class Self, class ETag = ExprTag,
        typename enable_if<is_base_of<ExprTag, ETag>::value>::type * = nullptr>
struct ExpressionAbstract : ETag, HashableStored<ExpressionAbstract<Self, ETag>>
{
    bool operator==(const Expression<ETag> &b) const
    {
        if(typeid(b.expr) == typeid(Self))
            return equals(static_cast<const Self &>(b));
        return false;
    }
};

template<class Self, class ETag = ExprTag>
struct Unary : ExpressionAbstract<Self, ETag>
{
    shared_ptr<ETag> e;

    Unary(Expression<ETag> f) : e(f.expr)
    {}

    virtual ~Unary() = default;

    virtual bool equals(const Self &b) const
    {
        return *e == *b.e;
    }
};

template<class Self, class ETag = ExprTag, bool associative = false>
struct Binary : ExpressionAbstract<Self, ETag>
{
    shared_ptr<ETag> l, r;

    Binary(Expression<ETag> l, Expression<ETag> r) : l(l.expr), r(r.expr)
    {}

    virtual ~Binary() = default;

    virtual bool equals(const Self &b) const
    {
        if((*l == *b.l) && (*r == *b.r))
            return true;
        return (associative && (*l == *b.r) && (*r == *b.l));
    }
};

template<class ETag = ExprTag>
struct ExpressionWrapper : Expression<ETag>
{
    ExpressionWrapper(const ExpressionWrapper &) = delete;

    operator Expression<ETag>() const
    {
        return Expression<ETag>(this->expr);
    }
};
