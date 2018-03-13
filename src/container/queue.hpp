#pragma once

#include "../other/arraymethods.hpp"
#include "../other/defdef.h"
#include "persistancequeue.hpp"
#include "stack.hpp"

#include <algorithm>
#include <stdio.h>

using std::pair;

template<typename T>
struct queue
{
    struct block
    {
        T *array;
        block *next;

        explicit block(unsigned blocksize): array(new T[blocksize]), next(nullptr)
        {}
    };

    //blocksize - amount of elements in one block, len - current len of the queue
    //block = [element * blocksize, pointer to next block]
    const unsigned blocksize;
    unsigned len;

    //pointer to (head - first element, tail - last element, headlast - last element of first block,
    //taillast - last element of last block)
    block *head, *tail;
    unsigned headlast, taillast;

    explicit queue(unsigned blocksize = 300): blocksize((blocksize > 0) ? blocksize : 1), len(0),
                                              head(nullptr), tail(nullptr), headlast(0), taillast(0)
    {}

    ~queue()
    {
        while(head)
        {
            block *to_del = head;
            head = head->next;
            delete to_del;
        }
    }

    void push(T x)
    {
        if(taillast == 0)
        {
            auto *new_last = new block(blocksize);
            if(tail)
                tail->next = new_last;
            tail = new_last;
            if(!head)
                head = tail;
        }
        tail->array[taillast++] = x;
        if(taillast == blocksize)
            taillast = 0;
        len++;
    }

    T &pop()
    {
        ASSERT(len > 0);
        ASSERT(head);
        if(headlast == blocksize)
        {
            auto *new_head = head->next;
            delete head;
            head = new_head;
            ASSERT(head);
            headlast = 0;
        }
        len--;
        return head->array[headlast++];
    }

//    void display()
//    {
//        if(len == 0)
//            puts("Queue is empty");
//        else
//        {
//            block *b_ptr = head;
//            while(b_ptr->next)
//
//            char *pointer = head;
//            char *pointer_last = headlast;
//            _tshow(*(T *) pointer);
//            pointer += sizeof(T);
//            while(pointer != tail)
//            {
//                if(pointer == pointer_last)
//                {
//                    pointer = *((char **) pointer_last);
//                    pointer_last = pointer + blocksize * sizeof(T);
//                }
//                fputs("->", stdout);
//                _tshow(*(T *) pointer);
//                pointer += sizeof(T);
//            }
//            putchar('\n');
//        }
//    }
};

template<typename T>
struct queuem
{
    stackm<T> v1, v2;
    unsigned len;

    queuem(unsigned size = 20, unsigned minsize = 20, int resizekf = 3)
            : v1(stackm<T>(size, minsize, resizekf)), v2(stackm<T>(size, minsize, resizekf)), len(0)
    {}

    ~queuem()
    {}

    T min()
    {
        if(v1.pointer < 0)
            return v2.min;
        else if(v2.pointer < 0)
            return v1.min;
        else
        {
            T x1 = v1.min;
            T x2 = v2.min;
            return x1 < x2 ? x1 : x2;
        }
    }

    void push(T x)
    {
        v1.push(x);
        len++;
    }

    T pop()
    {
        len--;
        if(v2.pointer >= 0)
            return v2.pop();
        else
        {
            while(v1.pointer > 0)
                v2.push(v1.pop());
            return v1.pop();
        }
    }

    void display(bool cmin = true)
    {
        if(v1.pointer + v2.pointer == -2)
            puts("Queue is empty");
        else
        {
            if(v1.pointer >= 0)
            {
                T mn = v1.m[0];
                vect<T *> v;
                int i = 0;
                while(i <= v1.pointer)
                {
                    v.add(v1.m + i);
                    if(v1.m[i + 1] == mn)
                    {
                        i++;
                        mn = v1.m[++i];
                    } else
                        i++;
                }
                for(int i = v.maxs; i > 0; i--)
                {
                    _tshow(*v.get(i));
                    fputs("->", stdout);
                }
            }
            if(v2.pointer >= 0)
            {
                T mn = v2.m[0];
                int i;
                if(v1.pointer < 0)
                {
                    _tshow(v2.m[0]);
                    if(v2.m[1] == mn)
                    {
                        i = 2;
                        mn = v2.m[2];
                    } else
                        i = 1;
                } else
                    i = 0;
                while(i <= v2.pointer)
                {
                    fputs("->", stdout);
                    _tshow(v2.m[i]);
                    if(v2.m[i + 1] == mn)
                    {
                        i++;
                        mn = v2.m[++i];
                    } else
                        i++;
                }
            }
            putchar('\n');
            if(cmin)
            {
                fputs("Min:", stdout);
                _tshow(min());
                putchar('\n');
            }
        }
    }
};

template<typename T>
struct prqueue
{
    vect<unsigned> linka, linkl;
    vect<pair<int, T> > q;
    unsigned len, posn;

    prqueue() : len(0), posn(0)
    {}

    void moveup(unsigned p)
    {
        unsigned p2;
        while(p > 0)
        {
            p2 = (p - 1) / 3;
            if(q.get(p2).first < q.get(p).first)
            {
                linkl.swap(linka[p], linka[p2]);
                linka.swap(p, p2);
                q.swap(p, p2);
            } else
                break;
            p = p2;
        }
    }

    void movedown(unsigned p = 0)
    {
        unsigned p2, p3;
        p2 = p3 = p * 3 + 1;
        while(p2 <= len)
        {
            if(++p2 <= len)
            {
                if(q.get(p2).first > q.get(p3).first)
                    p3 = p2;
                if((++p2 <= len) and (q.get(p2).first > q.get(p3).first))
                    p3 = p2;
            }
            if(q.get(p3).first > q.get(p).first)
            {
                linkl.swap(linka[p], linka[p3]);
                linka.swap(p, p3);
                q.swap(p, p3);
            } else
                break;
            p = p2;
            p2 = p3 = p * 3 + 1;
        }
    }

    void push(T c, int priority = 0)
    {
        q.set(len, make_pair(priority, c));
        linka[posn] = len;
        linkl[len] = posn++;
        this->moveup(len++);
    }

    int popmaxpriority(T &c)
    {
        if(len > 0)
        {
            c = q.get(0).second;
            int ret = q.get(0).first;
            linkl[0] = linkl[--len];
            q[0] = q.get(len);
            this->movedown();
            return ret;
        } else
            return -1;
    }

    int getmaxpriority(T &c)
    {
        if(len > 0)
        {
            c = q.get(0).second;
            return q.get(0).first;
        } else
            return -1;
    }

    void setPriority(unsigned n, int prioritydif)
    {
        unsigned pos = linkl[n];
        q.set(pos, make_pair(q.get(pos).first + prioritydif, q.get(pos).second));
        if(prioritydif > 0)
            this->moveup(pos);
        else
            this->movedown(pos);
    }
};
