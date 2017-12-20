#pragma once

template<typename T>
struct oneLinkedList
{
    struct node
    {
        //pointer to data
        T data;
        //pointer to next node
        node *next;

        node(T data, node *next = nullptr) : data(data), next(next)
        {}

        ~node() = default;
    };

    //first, last and detour(for cycling throw)
    node *first, *last;

    oneLinkedList() : first(nullptr), last(nullptr)
    {}

    //insert new node after now
    void insert(node *now, T data)
    {
        now->next = new node(data, now->next);
        if(last == now)
            last = now->next;
    }

    //insert new node to the front
    void insert_front(T data)
    {
        if(first == nullptr)
            first = last = new node(data);
        else
            first = new node(data, first);
    }

    //insert new node to the tail
    void insert_tail(T data)
    {
        if(first == nullptr)
            first = last = new node(data);
        else
            last->next = new node(data, last->next);
    }

    T removeNext(node *beforeDel)
    {
        if(!beforeDel)
            return remove();
        T data = beforeDel->next->data;
        node *nextToNext = beforeDel->next->next;
        delete beforeDel->next;
        beforeDel->next = nextToNext;
        return data;
    }

    //remove first node and return it's data
    T remove()
    {
        if(!first)
            throw 56;
        T data = first->data;
        node *second = first->next;
        delete first;
        first = second;
        return data;
    }
};

template<typename T>
struct dllist
{
    struct clist
    {
        T c;
        clist *n, *p;

        clist(T c, clist *n = 0, clist *p = 0) : c(c), n(n), p(p)
        {}

        ~clist()
        {}
    };

    clist *t, *h;
    unsigned len;

    dllist() : t(0), h(0), len(0)
    {}

    ~dllist()
    {
        T *s = h, *ns;
        while(s != 0)
        {
            ns = s->n;
            delete s;
            s = ns;
        }
    }

    void pushtail(T x)
    {
        clist e(x, 0, t);
        if(t == 0)
            h = *e;
        else
            t->n = *e;
        t = *e;
        len++;
    }

    void pushfront(T x)
    {
        clist e(x, h, 0);
        if(h == 0)
            t = *e;
        else
            h->p = *e;
        h = *e;
        len++;
    }

    void push(unsigned p, T x)
    {
        if(p <= len)
        {
            if(p >= (len + 1) / 2)
            {
                p = len - p;
                clist *k = t;
                while(p-- > 0)
                {
                    k = k->p;
                }
                clist e(x);
                if(k == 0)
                {
                    len = 1;
                    h = *e;
                    t = *e;
                    return;
                }
                e.p = k;
                e.n = k->n;
                if(k->n != 0)
                    k->n->p = *e;
                else
                    t = *e;
                k->n = *e;
                len++;
            } else
            {
                clist *k = h;
                while(p-- > 0)
                {
                    k = k->n;
                }
                clist e(x, k, k->p);
                if(k->p != 0)
                    k->p->n = *e;
                else
                    h = *e;
                k->p = *e;
                len++;
            }
        }
    }

    T poptail()
    {
        if(len > 0)
        {
            T x = t->c;
            clist *k = t->p;
            if(k != 0)
                k->n = 0;
            else
                h = 0;
            delete t;
            t = k;
            len--;
            return x;
        } else
            return -1;
    }

    T popfront()
    {
        if(h > 0)
        {
            T x = h->c;
            clist *k = h->n;
            if(k != 0)
                k->p = 0;
            else
                t = 0;
            delete h;
            h = k;
            len--;
            return x;
        } else
            return -1;
    }

    T pop(unsigned p)
    {
        if(p < len)
        {
            if(p >= (len + 1) / 2)
            {
                p = len - p;
                clist *k = t;
                while(p-- > 1)
                {
                    k = k->p;
                }
                T x = k->c;
                if(k->n != 0)
                    k->n->p = k->p;
                else
                    t = k->p;
                k->p->n = k->n;
                delete k;
                len--;
                return x;
            } else
            {
                clist *k = h;
                while(p-- > 0)
                {
                    k = k->n;
                }
                T x = k->c;
                if(k->n == 0)
                {
                    delete k;
                    t = 0;
                    h = 0;
                    len = 0;
                    return x;
                }
                if(k->p != 0)
                    k->p->n = k->n;
                else
                    h = k->n;
                k->n->p = k->p;
                delete k;
                len--;
                return x;
            }
        } else
            return -1;
    }

    T get(unsigned p)
    {
        if(p < len)
        {
            clist *k;
            if(p >= (len + 1) / 2)
            {
                p = len - p;
                k = t;
                while(p-- > 1)
                {
                    k = k->p;
                }
            } else
            {
                k = h;
                while(p-- > 0)
                {
                    k = k->n;
                }
            }
            return k->c;
        } else
            return -1;
    }

    void set(unsigned p, T x)
    {
        if(p < len)
        {
            clist *k;
            if(p >= (len + 1) / 2)
            {
                p = len - p;
                k = t;
                while(p-- > 1)
                {
                    k = k->p;
                }
            } else
            {
                k = h;
                while(p-- > 0)
                {
                    k = k->n;
                }
            }
            k->c = x;
        }
    }
};
