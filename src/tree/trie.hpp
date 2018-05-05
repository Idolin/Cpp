#pragma once

#include <limits>
#include "../struct/optional.hpp"
#include "../template/valuemethods.hpp"
#include "../template/arraymethods.hpp"

template<typename TKey = char, typename TData = none,
        typename = typename std::enable_if<(sizeof(TKey) <= 2)>::type,
        typename = typename std::enable_if<std::is_integral<TKey>::value>::type>
struct trie
{
private:
    struct node
    {
        node *arr[1u << (sizeof(TKey) * 8)];
        TData data;
        bool term;

        node(): arr(), term(false)
        {}

        ~node()
        {
            for(auto &i: arr)
                delete i;
        }

        node*& operator[](unsigned index)
        {
            return arr[index];
        }

        const node *operator[](unsigned index) const
        {
            return arr[index];
        }
    };

    node root;

public:
    trie(): root(node())
    {}

    ~trie() = default;

    bool insert(const TKey *s, unsigned len, const TData data = 0, bool rewrite = false)
    {
        node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            auto index = _valueMethods<TKey>::to_unsigned(s[i]);
            if(!(*n)[index])
                (*n)[index] = new node();
            n = (*n)[index];
        }
        if(!n->term || rewrite)
        {
            rewrite = n->term;
            n->term = true;
            n->data = data;
            return rewrite;
        }
        return false;
    }

    optional<TData> findKey(const TKey *s, unsigned len) const
    {
        const node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            auto index = _valueMethods<TKey>::to_unsigned(s[i]);
            if(!(*n)[index])
                return optional<TData>();
            n = (*n)[index];
        }
        if(!n->term)
            return optional<TData>();
        return optional<TData>(n->data);
    }

    optional<TData> erase(const TKey *s, unsigned len)
    {
        node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            auto index = _valueMethods<TKey>::to_unsigned(s[i]);
            if(!(*n)[index])
                return optional<TData>();
            n = (*n)[index];
        }
        if(n->term)
        {
            n->term = false;
            TData data = std::move(n->data);
            n->data = TData();
            return optional<TData>(std::move(data));
        }
        return optional<TData>();
    }
};

template<typename TData>
struct trie<char, TData>
{
private:
    struct node
    {
        node *arr[1u << (sizeof(char) * 8)];
        TData data;
        bool term;

        node(): arr(), term(false)
        {}

        ~node()
        {
            for(auto &i: arr)
                delete i;
        }

        node*& operator[](unsigned index)
        {
            return arr[index];
        }

        const node *operator[](unsigned index) const
        {
            return arr[index];
        }
    };

    node root;

public:
    trie(): root(node())
    {}

    ~trie() = default;

    bool insert(const char *s, const TData data = 0, bool rewrite = false,
                unsigned len = std::numeric_limits<unsigned>::max())
    {
        node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            if(len == std::numeric_limits<unsigned>::max() && s[i] == '\0')
                break;
            auto index = _valueMethods<char>::to_unsigned(s[i]);
            if(!(*n)[index])
                (*n)[index] = new node();
            n = (*n)[index];
        }
        if(!n->term || rewrite)
        {
            rewrite = n->term;
            n->term = true;
            n->data = data;
            return rewrite;
        }
        return false;
    }

    optional<TData> findKey(const char *s,
                            unsigned len  = std::numeric_limits<unsigned>::max()) const
    {
        const node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            if(len == std::numeric_limits<unsigned>::max() && s[i] == '\0')
                break;
            auto index = _valueMethods<char>::to_unsigned(s[i]);
            if(!(*n)[index])
                return optional<TData>();
            n = (*n)[index];
        }
        if(!n->term)
            return optional<TData>();
        return optional<TData>(n->data);
    }

    optional<TData> erase(const char *s,
                          unsigned len = std::numeric_limits<unsigned>::max())
    {
        node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            if(len == std::numeric_limits<unsigned>::max() && s[i] == '\0')
                break;
            auto index = _valueMethods<char>::to_unsigned(s[i]);
            if(!(*n)[index])
                return optional<TData>();
            n = (*n)[index];
        }
        if(n->term)
        {
            n->term = false;
            TData data = std::move(n->data);
            n->data = TData();
            return optional<TData>(std::move(data));
        }
        return optional<TData>();
    }
};

template<>
struct trie<char, none>
{
private:
    struct node
    {
        node *arr[1u << (sizeof(char) * 8)];
        bool term;

        node(): arr(), term(false)
        {}

        ~node()
        {
            for(auto &i: arr)
                delete i;
        }

        node*& operator[](unsigned index)
        {
            return arr[index];
        }

        const node *operator[](unsigned index) const
        {
            return arr[index];
        }
    };

    node root;

public:
    trie(): root(node())
    {}

    ~trie() = default;

    bool insert(const char *s, bool rewrite = false,
                unsigned len = std::numeric_limits<unsigned>::max())
    {
        node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            if(len == std::numeric_limits<unsigned>::max() && s[i] == '\0')
                break;
            auto index = _valueMethods<char>::to_unsigned(s[i]);
            if(!(*n)[index])
                (*n)[index] = new node();
            n = (*n)[index];
        }
        if(!n->term || rewrite)
        {
            rewrite = n->term;
            n->term = true;
            return rewrite;
        }
        return false;
    }

    bool findKey(const char *s,
                 unsigned len  = std::numeric_limits<unsigned>::max()) const
    {
        const node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            if(len == std::numeric_limits<unsigned>::max() && s[i] == '\0')
                break;
            auto index = _valueMethods<char>::to_unsigned(s[i]);
            if(!(*n)[index])
                return false;
            n = (*n)[index];
        }
        return n->term;
    }

    bool erase(const char *s,
               unsigned len = std::numeric_limits<unsigned>::max())
    {
        node *n = &root;
        for(unsigned i = 0;i < len;i++)
        {
            if(len == std::numeric_limits<unsigned>::max() && s[i] == '\0')
                break;
            auto index = _valueMethods<char>::to_unsigned(s[i]);
            if(!(*n)[index])
                return false;
            n = (*n)[index];
        }
        if(n->term)
        {
            n->term = false;
            return true;
        }
        return false;
    }
};