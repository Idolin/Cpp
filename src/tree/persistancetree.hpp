#pragma once

#include "../other/defdef.h"
#include "../container/vector.hpp"

#include <algorithm>

using std::pair;

template<typename T>
struct ptree
{
    struct etree
    {
        T c;
        etree *left, *right;

        etree(T c) : c(c), left(0), right(0)
        {}

        etree(etree *e) : left(e->left), right(e->right)
        {}
    };

    vect <pair<etree *, unsigned>> tops;

    ptree()
    {
        tops.set(0, pair<etree *, unsigned>(0, 0));
    }

    T get(unsigned p, unsigned v)
    {
        etree *top = tops.get(v).first;
        unsigned char r;
        unsigned height = tops.get(v).second;
        while((height > 1) && (top != 0))
        {
            height /= 3;
            r = p / height;
            p %= height;
            if(r == 0)
                top = top->c1;
            elif(r == 1)
                top = top->c2;
            else
                top = top->c3;
        }
        return (top != 0) ? top->c : -1;
    }

    void set(T c, unsigned p, unsigned v, bool newversion = true)
    {
        etree *top = tops.get(v).first;
        unsigned height = tops.get(v).second;
        etree *topnew;
        unsigned char r;
        if(top == 0)
        {
            top = new etree(c);
            height = 1;
            while(p)
            {
                topnew = new etree(c);
                r = p % 3;
                p /= 3;
                if(r == 0)
                    topnew->c1 = top;
                elif(r == 1)
                    topnew->c2 = top;
                else
                    topnew->c3 = top;
                top = topnew;
                height *= 3;
            }
            if(newversion)
                tops.add(make_pair(top, height));
        } else
        {
            if(height <= p)
            {
                newversion = false;
                while(height <= p)
                {
                    topnew = new etree(c);
                    height *= 3;
                    topnew->c1 = top;
                    top = topnew;
                }
                tops.add(make_pair(top, height));
            } else if(newversion)
            {
                top = new etree(top);
                tops.add(make_pair(top, height));
            }
            while(height > 1)
            {
                height /= 3;
                r = p / height;
                p %= height;
                if(r == 0)
                {
                    if(top->c1 == 0)
                    {
                        top->c1 = new etree(c);
                        newversion = false;
                    }
                    if(newversion)
                        top->c1 = new etree(top->c1);
                    top = top->c1;
                } elif(r == 1)
                {
                    if(top->c2 == 0)
                    {
                        top->c2 = new etree(c);
                        newversion = false;
                    }
                    if(newversion)
                        top->c2 = new etree(top->c2);
                    top = top->c2;
                } else
                {
                    if(top->c3 == 0)
                    {
                        top->c3 = new etree(c);
                        newversion = false;
                    }
                    if(newversion)
                        top->c3 = new etree(top->c3);
                    top = top->c3;
                }
            }
        }
        top->c = c;
    }
};
