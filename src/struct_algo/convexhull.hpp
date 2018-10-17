#pragma once

#include "../container/stack.hpp"

template<typename T>
struct convexHull
{
    struct linep
    {
        T c, k, h;

        linep()
        {}

        linep(T c, T k, T h) : c(c), k(k), h(h)
        {}
    };

    stack <linep> *s;

    convexHull() : s(new stack<linep>())
    {}

    void putLine(T o, T k)
    {
        if(s->pointer == 0)
            s->push(linep(0, k, 0));
        else
        {
            T c = getMin(o);
            s->pointer--;
            T h = (c - s->m[s->pointer].c) / (s->m[s->pointer].k - k);
            while((s->pointer > 0) and (h <= s->m[s->pointer].h))
            {
                s->pointer--;
                h = (c - s->m[s->pointer].c) / (s->m[s->pointer].k - k);
            }
            s->pointer++;
            s->push(linep(c, k, h));
        }
    }

    T getMin()
    {
        return s->pointer > 0 ? s->peek().c : 0;
    }

    T getMin(T h)
    {
        unsigned leftBorder = 0, rightBorder = s->pointer - 1;
        while(leftBorder < rightBorder)
        {
            unsigned middle = leftBorder + (rightBorder - leftBorder + 1) / 2;
            if(h > s->m[middle].h)
                leftBorder = middle;
            else
                rightBorder = middle - 1;
        }
        return s->m[rightBorder].c + s->m[rightBorder].k * h;
    }
};
