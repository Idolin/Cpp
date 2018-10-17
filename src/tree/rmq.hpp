#pragma once

#include "../other/commonmethods.hpp"

#include <algorithm>

using std::min;
using std::max;

template<typename T>
struct rmq
{
    T *tree, *m, neutral;
    unsigned lent, lenm;

    rmq(unsigned len, T n = 0, bool fill = true) : neutral(n), lenm(to2(len) - 1)
    {
        lent = lenm * 2 + 1;
        tree = new T[lent];
        m = tree + lenm;
        if(fill)
        {
            for(unsigned u = 0; u <= lenm; u++)
                m[u] = neutral;
            this->init();
        } else
            for(unsigned u = len; u <= lenm; u++)
                m[u] = neutral;
    }

    ~rmq()
    {
        delete[] tree;
    }

    T calc(T x1, T x2)
    {
        return x1 + x2;
    }

    void init()
    {
        for(unsigned i = lent - 1; i > 1; i -= 2)
            tree[i / 2 - 1] = calc(tree[i - 1], tree[i]);
    }

    void set(unsigned p, T x)
    {
        T n = x;
        p += lenm;
        for(; p > 0; p = (p - 1) / 2)
        {
            tree[p] = n;
            if(p % 2 == 1)
                n = calc(n, tree[p + 1]);
            else
                n = calc(tree[p - 1], n);
        }
        tree[0] = n;
    }

    T get(unsigned l, unsigned r)
    {
        l += lenm;
        r += lenm;
        T ret1 = neutral;
        T ret2 = neutral;
        while(l < r)
        {
            if(l % 2 == 0)
                ret1 = calc(ret1, tree[l]);
            if(r % 2 == 1)
                ret2 = calc(tree[r], ret2);
            l /= 2;
            r = r / 2 - 1;
        }
        if(l == r)
            ret1 = calc(ret1, tree[l]);
        return calc(ret1, ret2);
    }
};

template<typename T>
struct rmqrs
{
    T *m, neutral;
    // merge(tneutral, x) = x
    T *tree, tneutral;
    unsigned lent, lenm;

    rmqrs(unsigned len, T nodeNeutral = 0, T n = 0, bool fill = true) : neutral(n),
                                                                        tneutral(nodeNeutral), lent(to2(len) - 1),
                                                                        lenm(len)
    {
        tree = new T[lent * 2 + lenm + (lenm & 1)];
        m = tree + lent * 2;
        if(lenm & 1)
            m[lenm] = neutral;
        if(fill)
        {
            for(unsigned u = 0; u < lent; u++)
            {
                tree[u * 2] = neutral;
                tree[u * 2 + 1] = tneutral;
            }
            for(unsigned u = 0; u < lenm; u++)
                m[u] = neutral;
        }
    }

    ~rmqrs()
    {
        delete[] tree;
    }

    //recalc value for new interval
    T recalcValue(T x, T r, unsigned lb, unsigned rb)
    {
        return x + r;
    }

    // merge two values
    T calcValue(T x1, T x2)
    {
        return x1 + x2;
    }

    // merge two values
    // merge(tneutral, x) = x
    T merge(T x1, T x2)
    {
        return x1 + x2;
    }

    T init(unsigned node = 0)
    {
        if(node >= lent)
        {
            unsigned offset = node - lent;
            if(offset >= lenm)
                return neutral;
            else
                return m[offset];
        }
        tree[node * 2] = calcValue(init(node * 2 + 1), init(node * 2 + 2));
        tree[node * 2 + 1] = tneutral;
        return tree[node * 2];
    }

    void set(unsigned l, unsigned r, T x)
    {
        this->set(l, r, x, 0, 0, lent);
    }

    void set(unsigned l, unsigned r, T x, unsigned node, unsigned ln, unsigned rn)
    {
        if(node >= lent)
            m[node - lent] = recalcValue(m[node - lent], x, l, r);
        else
        {
            if((ln == l) && (rn == r))
            {
                if(tree[node * 2 + 1] == tneutral)
                    tree[node * 2 + 1] = x;
                else
                    tree[node * 2 + 1] = merge(tree[node * 2 + 1], x);
                tree[node * 2] = recalcValue(tree[node * 2], x, l, r);
            } else
            {
                unsigned mn = ln + (rn - ln) / 2;
                if(tree[node * 2 + 1] != tneutral)
                {
                    this->set(ln, mn, tree[node * 2 + 1], node * 2 + 1, ln, mn);
                    this->set(mn + 1, rn, tree[node * 2 + 1], node * 2 + 2, mn + 1, rn);
                    tree[node * 2 + 1] = tneutral;
                }
                if(l <= mn)
                    this->set(l, min(mn, r), x, node * 2 + 1, ln, mn);
                if(r > mn)
                    this->set(max(mn + 1, l), r, x, node * 2 + 2, mn + 1, rn);
                if(node * 2 + 1 >= lent)
                    tree[node * 2] = calcValue(m[node * 2 + 1 - lent], m[node * 2 + 2 - lent]);
                else
                    tree[node * 2] = calcValue(tree[(node * 2 + 1) * 2], tree[(node * 2 + 2) * 2]);
            }
        }
    }

    T get(unsigned l, unsigned r)
    {
        return this->get(l, r, 0, 0, lent);
    }

    T get(unsigned l, unsigned r, unsigned node, unsigned ln, unsigned rn)
    {
        if(node >= lent)
            return m[node - lent];
        if((ln == l) && (rn == r))
            return tree[node * 2];
        unsigned mn = ln + (rn - ln) / 2;
        if(tree[node * 2 + 1] != tneutral)
        {
            this->set(ln, mn, tree[node * 2 + 1], node * 2 + 1, ln, mn);
            this->set(mn + 1, rn, tree[node * 2 + 1], node * 2 + 2, mn + 1, rn);
            tree[node * 2 + 1] = tneutral;
        }
        if(r <= mn)
            return get(l, r, node * 2 + 1, ln, mn);
        if(l > mn)
            return get(l, r, node * 2 + 2, mn + 1, rn);
        return calcValue(get(l, mn, node * 2 + 1, ln, mn), get(mn + 1, r, node * 2 + 2, mn + 1, rn));
    }

    void display()
    {
        for(unsigned i = 0; i < lent; i++)
        {
            if(tree[i * 2] == neutral)
                putchar('=');
            else
                _tshow(tree[i * 2]);
            if(tree[i * 2 + 1] == tneutral)
                fputs("=, ", stdout);
            else
            {
                _tshow(tree[i * 2 + 1]);
                fputs(", ", stdout);
            }
            if(tree[i * 2 + 1] == tneutral)
                fputs("= | ", stdout);
            else
            {
                _tshow(tree[i * 2 + 1]);
                fputs(" | ", stdout);
            }
        }
        for(unsigned i = 0; i < lenm - 1; i++)
            if(m[i] == neutral)
                fputs("= | ", stdout);
            else
            {
                _tshow(m[i]);
                fputs(" | ", stdout);
            }
        if(m[lenm - 1] == neutral)
            putchar('=');
        else
        {
            _tshow(m[lenm - 1]);
            putchar('\n');
        }
    }
};

template<typename T>
struct rmqr
{
    T *m, neutral;  //m - array, neutral - neutral element
    T *tree, *tneutral; //tree - tree, tneutral - neutral request
    bool *neutralValue; //which of elements in tree are neutral
    unsigned lent, lenm, k; //k - amount of elements in request
    rmqr(unsigned len, T *nodeNeutral, unsigned k = 1, T n = 0, bool fill = true) : neutral(n), tneutral(nodeNeutral),
                                                                                    lent(to2(len) - 1), lenm(len),
                                                                                    k(k + 1)
    {
        tree = new T[lent * (this->k) + lenm + lenm % 2];
        neutralValue = new bool[lent];
        m = tree + lent * (this->k);
        if(lenm % 2 == 1)
            m[lenm] = neutral;
        if(fill)
        {
            for(unsigned u = 0; u < lent; u++)
            {
                tree[u * (this->k)] = neutral;
                for(unsigned i = 1; i < this->k; i++)
                    tree[u * (this->k) + i] = tneutral[i - 1];
                neutralValue[u] = true;
            }
            for(unsigned u = 0; u < lenm; u++)
                m[u] = neutral;
        }
    }

    ~rmqr()
    {
        delete[] tree;
        delete[] neutralValue;
    }

    T recalcValue(T x, T *r, unsigned lb, unsigned rb)
    {
        if(r[1] != 0)
            return x + r[1];
        else if(r[0] != tneutral[0])
            return r[0];
        else
            return x;
    }

    T calcValue(T x1, T x2)
    {
        return x1 < x2 ? x1 : x2;
    }

    void merge(T *x1, T *x2)
    {
        if(x2[1] == 0)
        {
            if(x2[0] == neutral)
                return;
            x1[0] = x2[0];
            x1[1] = 0;
        } else if(x1[0] == neutral)
            x1[1] += x2[1];
        else
            x1[0] += x2[1];
    }

    T init(unsigned node = 0)
    {
        if(node >= lent)
        {
            unsigned offset = node - lent;
            if(offset >= lenm)
                return neutral;
            else
                return m[offset];
        }
        tree[node * k] = calcValue(init(node * 2 + 1), init(node * 2 + 2));
        for(unsigned i = 1; i < k; i++)
            tree[node * k + i] = tneutral[i - 1];
        neutralValue[node] = true;
        return tree[node * k];
    }

    void set(unsigned l, unsigned r, T *x)
    {
        this->set(l, r, x, 0, 0, lent);
    }

    void set(unsigned l, unsigned r, T *x, unsigned node, unsigned ln, unsigned rn)
    {
        if(node >= lent)
            m[node - lent] = recalcValue(m[node - lent], x, l, r);
        else
        {
            if((ln == l) and (rn == r))
            {
                if(neutralValue[node])
                    for(unsigned u = 1; u < k; u++)
                        tree[node * k + u] = x[u - 1];
                else
                    this->merge(tree + node * k + 1, x);
                neutralValue[node] = false;
                tree[node * k] = recalcValue(tree[node * k], x, l, r);
            } else
            {
                unsigned mn = ln + (rn - ln) / 2;
                if(not neutralValue[node])
                {
                    this->set(ln, mn, tree + node * k + 1, node * 2 + 1, ln, mn);
                    this->set(mn + 1, rn, tree + node * k + 1, node * 2 + 2, mn + 1, rn);
                    for(unsigned u = 1; u < k; u++)
                        tree[node * k + u] = tneutral[u - 1];
                    neutralValue[node] = true;
                }
                if(l <= mn)
                    this->set(l, min(mn, r), x, node * 2 + 1, ln, mn);
                if(r > mn)
                    this->set(max(mn + 1, l), r, x, node * 2 + 2, mn + 1, rn);
                if(node * 2 + 1 >= lent)
                    tree[node * k] = calcValue(m[node * 2 + 1 - lent], m[node * 2 + 2 - lent]);
                else
                    tree[node * k] = calcValue(tree[(node * 2 + 1) * k], tree[(node * 2 + 2) * k]);
            }
        }
    }

    T get(unsigned l, unsigned r)
    {
        return this->get(l, r, 0, 0, lent);
    }

    T get(unsigned l, unsigned r, unsigned node, unsigned ln, unsigned rn)
    {
        if(node >= lent)
            return m[node - lent];
        if((ln == l) and (rn == r))
            return tree[node * k];
        unsigned mn = ln + (rn - ln) / 2;
        if(not neutralValue[node])
        {
            this->set(ln, mn, tree + node * k + 1, node * 2 + 1, ln, mn);
            this->set(mn + 1, rn, tree + node * k + 1, node * 2 + 2, mn + 1, rn);
            for(unsigned u = 1; u < k; u++)
                tree[node * k + u] = tneutral[u - 1];
            neutralValue[node] = true;
        }
        if(r <= mn)
            return get(l, r, node * 2 + 1, ln, mn);
        if(l > mn)
            return get(l, r, node * 2 + 2, mn + 1, rn);
        return calcValue(get(l, mn, node * 2 + 1, ln, mn), get(mn + 1, r, node * 2 + 2, mn + 1, rn));
    }

    void display()
    {
        for(unsigned i = 0; i < lent; i++)
        {
            if(tree[i * 2] == neutral)
                putchar('=');
            else
                _tshow(tree[i * 2]);
            if(tree[i * 2 + 1] == tneutral)
                fputs("=, ", stdout);
            else
            {
                _tshow(tree[i * 2 + 1]);
                fputs(", ", stdout);
            }
            if(tree[i * 2 + 1] == tneutral)
                fputs("= | ", stdout);
            else
            {
                _tshow(tree[i * 2 + 1]);
                fputs(" | ", stdout);
            }
        }
        for(unsigned i = 0; i < lenm - 1; i++)
            if(m[i] == neutral)
                fputs("= | ", stdout);
            else
            {
                _tshow(m[i]);
                fputs(" | ", stdout);
            }
        if(m[lenm - 1] == neutral)
            putchar('=');
        else
        {
            _tshow(m[lenm - 1]);
            putchar('\n');
        }
    }
};

struct roundParking
{
    unsigned *tree;
    unsigned lent, lenm;

    roundParking(unsigned len) : lent(to2(len) - 1), lenm(len)
    {
        if(lent == 0)
            lent = 1;
        lent = lent * 2 + 1;
        tree = new unsigned[lent];
        unsigned i = lent - 1;
        for(; i >= lent / 2 + lenm; i--)
            tree[i] = 0;
        for(unsigned k = lenm; i >= lent / 2; i--, k--)
            tree[i] = k;
        for(;; i--)
        {
            tree[i] = max(tree[i * 2 + 1], tree[i * 2 + 2]);
            if(i == 0)
                break;
        }
    }

    ~roundParking()
    {
        delete[] tree;
    }

    unsigned enter(unsigned p)
    {
        if(tree[0] < p)
            p = 1;
        unsigned i = 0;
        while(i < lent / 2)
            if(tree[i * 2 + 1] >= p)
                i = i * 2 + 1;
            else
                i = i * 2 + 2;
        p = tree[i];
        this->set(i);
        return p;
    }

    bool exit(unsigned p)
    {
        if(p > lenm)
            return false;
        unsigned i = p - 1 + lent / 2;
        if(tree[i] != 0)
            return false;
        this->set(i);
        return true;
    }

    void set(unsigned i)
    {
        unsigned z = 0;
        if(tree[i] == 0)
            z = i - lent / 2 + 1;
        for(; i > 0; i = (i - 1) / 2)
        {
            tree[i] = z;
            if(i % 2 == 1)
            {
                if(z < tree[i + 1])
                    z = tree[i + 1];
            } else if(z == 0)
                z = tree[i - 1];
        }
        tree[0] = z;
    }
};
