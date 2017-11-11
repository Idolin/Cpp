#pragma once

#include "vector.hpp"

#include <stdio.h>

template<typename T>
struct pqueue
{
    struct pcqueue
    {
        T c;
        pcqueue *p;

        pcqueue(T c, pcqueue *p = 0) : c(c), p(p)
        {}

        ~pcqueue()
        {}
    };

    struct pclqueue
    {
        pcqueue *p;
        pclqueue *n;

        pclqueue() : p(0), n(0)
        {}

        pclqueue(pcqueue *p, pclqueue *n) : p(p), n(n)
        {}

        ~pclqueue()
        {}
    };

    struct pvqueue
    {
        unsigned len, llen, llen2;
        pcqueue *s, *t;
        pclqueue *n, *l;
        char own;

        pvqueue() : s(0), t(0), n(0), l(0), own(0)
        {}

        pvqueue(unsigned len, unsigned llen, unsigned llen2, pcqueue *s = 0,
                pcqueue *t = 0, pclqueue *n = 0, pclqueue *l = 0) :
                len(len), llen(llen), llen2(llen2), s(s), t(t), n(n), l(l), own(0)
        {}

        ~pvqueue()
        {
            if(own and 1)
                delete t;
            if(own and 2)
                delete n;
            if(own and 4)
                delete l;
        }
    };

    vect<pvqueue *> q;

    pqueue<T>()
    {
        pvqueue *v;
        q.set(0, v);
    }

    ~pqueue()
    {
        for(unsigned t = 0; t < q.maxs; t++)
            delete q.get(t);
        delete q;
    }

    void push(unsigned v, T x)
    {
        pvqueue *p = q.get(v);
        pcqueue *e(x, p->t);
        pvqueue *n(p->len + 1, p->llen, p->llen2, p->s == 0 ? e : p->s, e, p->n);
        if(n->llen * 2 < n->len - 2)
        {
            n->l = new pclqueue();
            n->own = 5;
            if(p->l == 0)
                n->l->p = n->t->p;
            else
                n->l->p = p->l->p->p;
            n->l->n = p->l;
            n->llen2++;
            if(n->n == 0 or n->n->p == n->l->p)
            {
                n->n = n->l;
                n->l = 0;
                n->own = 3;
                n->llen = n->llen2;
                n->llen2 = 0;
            }
        } else
            n->l = 0;
        q.add(n);
    }

    T pop(unsigned v)
    {
        pvqueue *p = q.get(v);
        T x = p->s->c;
        pvqueue n(p->len - 1, p->llen, p->llen2, 0, p->t, p->n, p->l);
        if((n.llen - 1 + (n.llen == 0)) * 2 < n.len - 2)
        {
            n.l = new pclqueue();
            n.own = 4;
            if(p->l == 0)
                n.l->p = n.t->p;
            else
                n.l->p = p->l->p->p;
            n.l->n = p->l;
            n.llen2++;
            if(n.n == 0 or n.n->p == n.l->p)
            {
                n.n = n.l;
                n.l = 0;
                n.own = 2;
                n.llen = n.llen2;
                n.llen2 = 0;
            }
        } else
            n.l = 0;
        if(p->n == 0)
        {
            if(p->s != p->t)
                n.s = p->t;
            n.t = n.s;
        } else
        {
            n.s = n.n->p;
            n.n = n.n->n;
            if(n.n == 0)
            {
                n.n = n.l;
                n.llen = n.llen2;
                n.llen2 = 0;
            } else
                n.llen--;
        }
        q.add(n);
        return x;
    }

    void display(unsigned v, bool clen = true)
    {
        pvqueue *p = q.get(v);
        if(p->s == 0)
            puts("Queue is empty");
        else
        {
            if(p->s != p->t)
            {
                _tshow(p->s->c);
                fputs("->", stdout);
            }
            pclqueue *l = p->s;
            if(p->n != 0)
            {
                l = p->n;
                while(l->n != 0)
                {
                    _tshow(l->p->c);
                    fputs("->", stdout);
                    l = l->n;
                }
                _tshow(l->p->c);
                fputs("->", stdout);
            }
            if(p->len > 2 + p->llen)
            {
                pcqueue *f = l->p;
                l = p->l;
                vect<T *> v;
                while(l->p->p != f)
                {
                    l = l->p;
                    v.add(*(l->p->c));
                }
                for(int t = v.maxs; t >= 0; t--)
                {
                    _tshow(*v.get(t));
                    fputs("->", stdout);
                }
                l = p->l;
                while(l->n != 0)
                {
                    _tshow(l->p->c);
                    fputs("->", stdout);
                    l = l->n;
                }
                _tshow(l->p->c);
                fputs("->", stdout);
            }
            _tshow(p->t->c);
            putchar('\n');
            if(clen)
                printf("Length:%u\n", p->len);
        }
    }
};
