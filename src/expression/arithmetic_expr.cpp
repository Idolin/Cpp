#include "arithmetic_expr.h"

Const::Const(double v) : v(v)
{}

uint64_t Const::hash() const
{
    return static_cast<uint64_t>(v);
}

bool Const::equals(const Const *b) const
{
    return (v == b->v);
}

double Const::operator()(double) const
{
    return v;
}

Var::Var()
{}

uint64_t Var::hash() const
{
    return 7623589;
}

bool Var::equals(const Var *b) const
{
    return true;
}

double Var::operator()(double x) const
{
    return x;
}

Negate::Negate(ExprAbstractA *f) : UnaryA(f)
{}

Negate::Negate(int k) : UnaryA(k)
{}

uint64_t Negate::hash() const
{
    return !(f->hash()) + 1;
}

double Negate::operator()(double x) const
{
    return -(*f)(x);
}

Abs::Abs(ExprAbstractA *f) : UnaryA(f)
{}

Abs::Abs(int k) : UnaryA(k)
{}

uint64_t Abs::hash() const
{
    return (f->hash() ^ 0x03000000007000f1) + 7;
}

double Abs::operator()(double x) const
{
    double r = (*f)(x);
    return (r < 0) ? -r : r;
}

Sqrt::Sqrt(ExprAbstractA *f) : UnaryA(f)
{}

Sqrt::Sqrt(int k) : UnaryA(k)
{}

uint64_t Sqrt::hash() const
{
    uint64_t hash_value = f->hash();
    return ((hash_value << 7) ^ (hash_value >> 3)) - 17;
}

double Sqrt::operator()(double x) const
{
    return sqrt((*f)(x));
}

Sin::Sin(ExprAbstractA *f) : UnaryA(f)
{}

Sin::Sin(int k) : UnaryA(k)
{}

uint64_t Sin::hash() const
{
    uint64_t hash_value = f->hash();
    return ((hash_value & 0x55555555aaaaaaaa) | ((!hash_value) & 0xaaaaaaaa55555555)) + 113;
}

double Sin::operator()(double x) const
{
    return sin((*f)(x));
}

Cos::Cos(ExprAbstractA *f) : UnaryA(f)
{}

Cos::Cos(int k) : UnaryA(k)
{}

uint64_t Cos::hash() const
{
    uint64_t hash_value = f->hash();
    return (((!hash_value) & 0x55555555aaaaaaaa) | (hash_value & 0xaaaaaaaa55555555)) + 119;
}

double Cos::operator()(double x) const
{
    return cos((*f)(x));
}

Tg::Tg(ExprAbstractA *f) : UnaryA(f)
{}

Tg::Tg(int k) : UnaryA(k)
{}

uint64_t Tg::hash() const
{
    uint64_t hash_value = f->hash();
    return (((!hash_value) & 0xf0f0f0f0f0f0f0f0) | (hash_value & 0x0f0f0f0f0f0f0f0f)) - 73;
}

double Tg::operator()(double x) const
{
    double v = (*f)(x);
    return sin(v) / cos(v);
}

Ctg::Ctg(ExprAbstractA *f) : UnaryA(f)
{}

Ctg::Ctg(int k) : UnaryA(k)
{}

uint64_t Ctg::hash() const
{
    uint64_t hash_value = f->hash();
    return ((hash_value & 0xf0f0f0f0f0f0f0f0) | ((!hash_value) & 0x0f0f0f0f0f0f0f0f)) - 331;
}

double Ctg::operator()(double x) const
{
    double v = (*f)(x);
    return cos(v) / sin(v);
}

Ln::Ln(ExprAbstractA *f) : UnaryA(f)
{}

Ln::Ln(int k) : UnaryA(k)
{}

uint64_t Ln::hash() const
{
    uint64_t hash_value = f->hash();
    return (((hash_value & 0xffffffff00000000) >> 32) ^ (hash_value & 0x00000000ffffffff)) + hash_value;
}

double Ln::operator()(double x) const
{
    return log((*f)(x));
}

Exp::Exp(ExprAbstractA *f) : UnaryA(f)
{}

Exp::Exp(int k) : UnaryA(k)
{}

uint64_t Exp::hash() const
{
    uint64_t hash_value = f->hash();
    return (((hash_value & 0xffffffff00000000) >> 32) ^ (hash_value & 0x00000000ffffffff)) - hash_value;
}

double Exp::operator()(double x) const
{
    return exp((*f)(x));
}

Add::Add(ExprAbstractA *left, ExprAbstractA *right) : BinaryA(left, right)
{}

Add::Add(int k, ExprAbstractA *f) : BinaryA(k, f)
{}

Add::Add(ExprAbstractA *f, int k) : BinaryA(f, k)
{}

uint64_t Add::hash() const
{
    uint64_t r_hash = r->hash();
    return l->hash() + (r_hash << 7) - r_hash - 9457;
}

double Add::operator()(double x) const
{
    return (*l)(x) + (*r)(x);
}

Sub::Sub(ExprAbstractA *left, ExprAbstractA *right) : BinaryA(left, right)
{}

Sub::Sub(int k, ExprAbstractA *f) : BinaryA(k, f)
{}

Sub::Sub(ExprAbstractA *f, int k) : BinaryA(f, k)
{}

uint64_t Sub::hash() const
{
    uint64_t l_hash = l->hash();
    return (l_hash >> 7) + l_hash - r->hash() + 13771;
}

double Sub::operator()(double x) const
{
    return (*l)(x) - (*r)(x);
}

Mult::Mult(ExprAbstractA *left, ExprAbstractA *right) : BinaryA(left, right)
{}

Mult::Mult(int k, ExprAbstractA *f) : BinaryA(k, f)
{}

Mult::Mult(ExprAbstractA *f, int k) : BinaryA(f, k)
{}

uint64_t Mult::hash() const
{
    uint64_t l_hash = l->hash(), r_hash = r->hash();
    return (l_hash ^ r_hash) + (l_hash >> 3) - (r_hash << 3) + 793841;
}

double Mult::operator()(double x) const
{
    return (*l)(x) * (*r)(x);
}

Div::Div(ExprAbstractA *left, ExprAbstractA *right) : BinaryA(left, right)
{}

Div::Div(int k, ExprAbstractA *f) : BinaryA(k, f)
{}

Div::Div(ExprAbstractA *f, int k) : BinaryA(f, k)
{}

uint64_t Div::hash() const
{
    uint64_t l_hash = l->hash(), r_hash = r->hash();
    return !(l_hash ^ r_hash) - (l_hash << 7) + (r_hash >> 7) - 891643;
}

double Div::operator()(double x) const
{
    return (*l)(x) / (*r)(x);
}

Pwr::Pwr(ExprAbstractA *left, ExprAbstractA *right) : BinaryA(left, right)
{}

Pwr::Pwr(int k, ExprAbstractA *f) : BinaryA(k, f)
{}

Pwr::Pwr(ExprAbstractA *f, int k) : BinaryA(f, k)
{}

uint64_t Pwr::hash() const
{
    uint64_t l_hash = l->hash(), r_hash = r->hash();
    return ((l_hash + r_hash) ^ ((l_hash << 32) + (r_hash >> 32) + (l_hash & r_hash))) + 1975679;
}

double Pwr::operator()(double x) const
{
    return pow((*l)(x), (*r)(x));
}
