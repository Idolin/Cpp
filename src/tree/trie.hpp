#pragma once

#include "avl.hxx"

template<typename T = char>
struct trie
{
    struct node
    {
        avl_tree<node *, T> c; //childrens
        bool f; //end-flag
        node() : f(false)
        {}

        ~node()
        {
            for(auto l: c)
                delete l;
        }
    };

    node t; //root
    trie() : t(node())
    {}

    ~trie()
    {}

    void insert(const T *const s, unsigned len)
    {
        node *n = &t;
        unsigned i = 0;
        while(i < len)
        {
            auto it = n->c.findKey(s[i]);
            if(it == n->c.end())
                break;
            i++;
            n = *it;
        }
        while(i < len)
            n = *n->c.insert(s[i++], node()).first;
        n->f = true;
    }

    bool check(const T *const s, unsigned len) const
    {
        const node *n = &t;
        for(unsigned i = 0; i < len; i++)
        {
            auto it = n->c.findKey(s[i]);
            if(it == n->c.end())
                return false;
            n = *it;
        }
        return n->f;
    }

    void remove(const T *const s, unsigned len)
    {
        node *n = &t;
        for(unsigned i = 0; i < len; i++)
        {
            auto it = n->c.findKey(s[i]);
            if(it == n->c.end())
                return;
            n = *it;
        }
        n->f = false;
    }
};

template<>
struct trie<char>
{
    struct node
    {
        node **c; //childrens
        bool f; //end-flag
        node() : c(new node *[256]), f(false)
        {}

        ~node()
        {
            for(unsigned i = 1; i < 256; i++)
                delete c[i];
            delete[] c;
        }
    };

    node t; //root
    trie() : t(node())
    {}

    ~trie()
    {}

    void insert(const char *const s)
    {
        node *n = &t;
        unsigned i = 0;
        while(s[i] != '\0')
        {
            if(n->c[(unsigned char) s[i]] == 0)
                break;
            n = n->c[(unsigned char) s[i++]];
        }
        while(s[i] != '\0')
        {
            n->c[(unsigned char) s[i]] = new node();
            n = n->c[(unsigned char) s[i]];
        }
        n->f = true;
    }

    bool check(const char *const s) const
    {
        const node *n = &t;
        unsigned i = 0;
        while(s[i] != '\0')
            if((n = n->c[(unsigned char) s[i++]]) == 0)
                return false;
        return n->f;
    }

    void remove(const char *const s)
    {
        node *n = &t;
        unsigned i = 0;
        while(s[i] != '\0')
            if((n = n->c[(unsigned char) s[i++]]) == 0)
                return;
        n->f = false;
    }

    void make_fsm()
    {

    }
};