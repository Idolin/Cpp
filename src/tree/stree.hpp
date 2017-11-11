#pragma once

#include "../debug/def_debug.h"

#include <algorithm>
#include <stdio.h>

using std::make_pair;
using std::pair;

template<typename TData, typename TKey>
struct search_tree_node
{
    TKey key;
    TData data;

    search_tree_node(TKey key, TData data) : key(key), data(data)
    {}

    virtual ~search_tree_node()
    {}

    virtual search_tree_node *getLeft() = 0;

    virtual search_tree_node *getRight() = 0;

    virtual search_tree_node *getParent() = 0;
};

template<typename TData, typename TKey, class S>
struct search_tree_node_crtp : search_tree_node<TData, TKey>
{
    S *left, *right, *parent;

    search_tree_node_crtp(TKey key, TData data, S *parent) : search_tree_node<TData, TKey>(key, data),
                                                             left(0), right(0), parent(parent)
    {
        DEBUGLVLMSG(8, "Adding new node");
        if(parent)
        {
            if(parent->key > key)
                parent->left = (S *) this;
            else
                parent->right = (S *) this;
        }
    }

    virtual ~search_tree_node_crtp()
    {
        delete left;
        delete right;
    }

    search_tree_node<TData, TKey> *getLeft()
    {
        return left;
    }

    search_tree_node<TData, TKey> *getRight()
    {
        return right;
    }

    search_tree_node<TData, TKey> *getParent()
    {
        return parent;
    }

    virtual void make_root_links(S *root)
    {
        S *current = root;
        while(current != this)
            if(current->key > this->key)
            {
                current->left->parent = current;
                current = current->left;
            } else
            {
                current->right->parent = current;
                current = current->right;
            }
    }

    virtual void remove(S *root)
    {
        delete this;
    }

    virtual void changeData(TData newData, S *root)
    {
        this->data = newData;
    }

    virtual void show_node() const
    {}
};

template<typename TData, typename TKey>
struct tree_iterator
{
    search_tree_node<TData, TKey> *current;
    bool is_end;

    tree_iterator(search_tree_node<TData, TKey> *node, bool end_iterator = false) : current(node), is_end(end_iterator)
    {}

    // Элемент на который сейчас ссылается итератор.
    // Разыменование итератора end() неопределено.
    // Разыменование невалидного итератора неопределено.
    TData const &operator*() const
    {
        return current->data;
    }

    // Переход к элементу со следующим по величине ключом.
    // Инкремент итератора end() неопределен.
    // Инкремент невалидного итератора неопределен.
    tree_iterator<TData, TKey> &operator++()
    {
        if(current->getRight())
        {
            current = current->getRight();
            while(current->getLeft())
                current = current->getLeft();
            return *this;
        }
        search_tree_node<TData, TKey> *before = current;
        while((current->getParent()) && (current->getParent()->getRight() == current))
            current = current->getParent();
        if(!(current = current->getParent()))
        {
            current = before;
            is_end = true;
        }
        return *this;
    }

    tree_iterator<TData, TKey> operator++(int)
    {
        tree_iterator<TData, TKey> r = *this;
        ++*this;
        return r;
    }

    // Переход к элементу со следующим по величине ключом.
    // Декремент итератора begin() неопределен.
    // Декремент невалидного итератора неопределен.
    tree_iterator<TData, TKey> &operator--()
    {
        if(is_end)
        {
            is_end = false;
            return *this;
        }
        if(current->getLeft())
        {
            current = current->getLeft();
            while(current->getRight())
                current = current->getRight();
            return *this;
        }
        while((current->getParent()) && (current->getParent()->getLeft() == current))
            current = current->getParent();
        current = current->getParent();
        return *this;
    }

    tree_iterator<TData, TKey> operator--(int)
    {
        tree_iterator<TData, TKey> r = *this;
        --*this;
        return r;
    }

    // Сравнение. Итераторы считаются эквивалентными если одни ссылаются на один и тот же элемент.
    // Сравнение с невалидным итератором не определено.
    // Сравнение итераторов двух разных контейнеров не определено.
    bool operator==(const tree_iterator<TData, TKey> i) const
    {
        return ((current == i.current) && (is_end == i.is_end));
    }

    bool operator!=(const tree_iterator<TData, TKey> i) const
    {
        return !(*this == i);
    }
};

template<typename TData, typename TKey = int>
struct search_tree_abstract
{
    typedef tree_iterator<TData, TKey> t_iterator;

    virtual ~search_tree_abstract()
    {}

    virtual std::pair<t_iterator, bool> insert(TKey, TData, bool = false) = 0;

    virtual t_iterator findKey(TKey) const = 0;

    virtual TData erase(t_iterator) = 0;

    virtual bool empty() const = 0;

    virtual void show() const = 0;

    virtual t_iterator begin() const = 0;

    virtual t_iterator end() const = 0;
};

template<typename TData, typename TKey, class S>
struct search_tree_abstract_crtp;

template<typename TData, typename TKey, class S>
bool check_binary_tree(search_tree_abstract_crtp<TKey, TData, S> *);

template<typename TData, typename TKey, class S>
struct search_tree_abstract_crtp : search_tree_abstract<TData, TKey>
{
protected:
    S *root;

    friend bool check_binary_tree<TData, TKey, S>(search_tree_abstract_crtp<TKey, TData, S> *);

public:
    search_tree_abstract_crtp() : root(0)
    {}

    virtual ~search_tree_abstract_crtp()
    {}

    virtual bool empty() const
    {
        return (root == 0);
    }

    virtual void show() const
    {
        for(unsigned level = 0; show_level(root, level); level++)
            putchar('\n');
        putchar('\n');
        fflush(stdout);
    }

private:
    bool show_level(S *node, unsigned level) const
    {
        if(level)
        {
            if(node)
            {
                bool ok = show_level(node->left, level - 1);
                putchar('|');
                if(!show_level(node->right, level - 1))
                    return ok;
                return true;
            } else
            {
                show_level(0, level - 1);
                putchar('|');
                show_level(0, level - 1);
                return false;
            }
        } else
        {
            if(node)
            {
                node->show_node();
                return ((node->left) || (node->right));
            } else
            {
                fputs("empty", stdout);
                return false;
            }
        }
    }
};

template<typename TData, typename TKey, class S>
bool check_binary_tree_node(search_tree_node_crtp<TKey, TData, S> *node)
{
    if(!node)
        return true;
    if((node->right) && (node->right->parent != node))
        return false;
    if((node->left) && (node->left->parent != node))
        return false;
    return (check_binary_tree_node(node->left) && check_binary_tree_node(node->right));
}

template<typename TData, typename TKey, class S>
bool check_binary_tree(search_tree_abstract_crtp<TKey, TData, S> *tree)
{
    return check_binary_tree_node(tree->root);
}
