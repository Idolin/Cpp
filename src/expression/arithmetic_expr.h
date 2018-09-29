#pragma once

#include "../debug/def_debug.h"
#include "expr.hpp"

struct ExprAbstractA: ExprAbstract
{
    virtual double operator()(double) const = 0;
};

template<class Self>
struct ExprA: Expr<Self, ExprAbstractA>
{};

struct Const : ExprA<Const>
{
    double v;

    Const(double v);

    uint64_t hash() const;

    bool equals(const Const *) const;

    double operator()(double) const;
};

struct Var : ExprA<Var>
{
    Var();

    uint64_t hash() const;

    bool equals(const Var *) const;

    double operator()(double x) const;
};

template<class Self>
struct UnaryA : Unary<Self, ExprAbstractA>
{
    UnaryA(ExprAbstractA *f) : Unary<Self, ExprAbstractA>(f)
    {}

    UnaryA(int k) : UnaryA(new Const(k))
    {}
};

struct Negate : UnaryA<Negate>
{
    explicit Negate(ExprAbstractA *f);

    explicit Negate(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Abs : UnaryA<Abs>
{
    explicit Abs(ExprAbstractA *f);

    explicit Abs(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Sqrt : UnaryA<Sqrt>
{
    explicit Sqrt(ExprAbstractA *f);

    explicit Sqrt(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Sin : UnaryA<Sin>
{
    explicit Sin(ExprAbstractA *f);

    explicit Sin(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Cos : UnaryA<Cos>
{
    explicit Cos(ExprAbstractA *f);

    explicit Cos(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Tg : UnaryA<Tg>
{
    explicit Tg(ExprAbstractA *f);

    explicit Tg(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Ctg : UnaryA<Ctg>
{
    explicit Ctg(ExprAbstractA *f);

    explicit Ctg(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Ln : UnaryA<Ln>
{
    explicit Ln(ExprAbstractA *f);

    explicit Ln(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Exp : UnaryA<Exp>
{
    explicit Exp(ExprAbstractA *f);

    explicit Exp(int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

template<class Self, bool associative = true>
struct BinaryA : Binary<Self, associative, ExprAbstractA>
{
    BinaryA(ExprAbstractA *left, ExprAbstractA *right) : Binary<Self, associative, ExprAbstractA>(left, right)
    {}

    BinaryA(int k, ExprAbstractA *f) : BinaryA(new Const(k), f)
    {}

    BinaryA(ExprAbstractA *f, int k) : BinaryA(f, new Const(k))
    {}
};

struct Add : BinaryA<Add, false>
{
    Add(ExprAbstractA *left, ExprAbstractA *right);

    Add(int k, ExprAbstractA *f);

    Add(ExprAbstractA *f, int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Sub : BinaryA<Sub>
{
    Sub(ExprAbstractA *left, ExprAbstractA *right);

    Sub(int k, ExprAbstractA *f);

    Sub(ExprAbstractA *f, int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Mult : BinaryA<Mult, false>
{
    Mult(ExprAbstractA *left, ExprAbstractA *right);

    Mult(int k, ExprAbstractA *f);

    Mult(ExprAbstractA *f, int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Div : BinaryA<Div>
{
    Div(ExprAbstractA *left, ExprAbstractA *right);

    Div(int k, ExprAbstractA *f);

    Div(ExprAbstractA *f, int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Pwr : BinaryA<Pwr>
{
    Pwr(ExprAbstractA *left, ExprAbstractA *right);

    Pwr(int k, ExprAbstractA *f);

    Pwr(ExprAbstractA *f, int k);

    uint64_t hash() const;

    double operator()(double x) const;
};

struct Expression : ExpressionAbstract<Expression, ExprAbstractA>
{
    Expression() : ExpressionAbstract(new Const(0))
    {
        //DEBUGMSG("new Expression ()");
    }

    Expression(ExprAbstractA *e) : ExpressionAbstract(e)
    {
        //DEBUGMSG("new Expression (e)");
    }

    void operator=(ExprAbstractA *e)
    {
        //DEBUGMSG("Expression = ExprAbstractA*");
        ExpressionAbstract<Expression, ExprAbstractA>::operator=(e);
    }

    virtual double operator()(double x) const
    {
        return (*e)(x);
    }
};
