#pragma once

#include "vector.hpp"

#include <algorithm>
#include <stdio.h>

template<typename T>
struct pstack
{
    struct cpstack
    {
        T c;
        cpstack *p;

        cpstack(T c, cpstack *n = 0) : c(c), p(n)
        {}

        ~cpstack()
        {
            delete p;
        }
    };

    vect<cpstack *> v;

    pstack() : v(vect<cpstack *>())
    {
        v.set(0, 0);
    }

    pstack(T x) : v(vect<cpstack *>())
    {
        v.set(0, 0);
        v.set(1, *cpstack(x, 0));
    }

    ~pstack()
    {
        for(unsigned t = 0; t < v.maxs; t++)
            delete v.get(t);
    }

    T pop(unsigned v)
    {
        cpstack *c = this->v.get(v);
        if(c != 0)
        {
            this->v.add(c->p);
            return c->c;
        } else
            return -1;
    }

    T pop()
    {
        cpstack *c = v.getl();
        if(c != 0)
        {
            this->v.add(c->p);
            return c->c;
        } else
            return -1;
    }

    T get(unsigned v)
    {
        cpstack *c = this->v.get(v);
        return c == 0 ? -1 : c->c;
    }

    T get()
    {
        cpstack *c = this->v.getl();
        return c == 0 ? -1 : c->c;
    }

    void push(unsigned v, T x)
    {
        cpstack c(x, this->v.get(v));
        this->v.add(&c);
    }

    void push(T x)
    {
        cpstack c(x, v.getl());
        v.add(&c);
    }

    void display(unsigned v)
    {
        cpstack *c = this->v.get(v);
        if(c == 0)
            puts("Stack is empty");
        else
        {
            _tshow(c->c);
            c = c->p;
            while(c != 0)
            {
                fputs("->", stdout);
                _tshow(c->c);
                c = c->p;
            }
            putchar('\n');
        }
    }
};
