#pragma once

#include "../other/defdef.h"
#include "stree.hpp"

#include <algorithm>


using std::enable_if;
using std::is_base_of;
using std::max;
using std::swap;

struct avl_node_tag
{};

template<typename TData, typename TKey, class S>
struct avl_node_crtp: search_tree_node_crtp<TData, TKey, S>, avl_node_tag
{
    unsigned char height;

    avl_node_crtp(TKey key, TData data, S *parent):
            search_tree_node_crtp<TData, TKey, S>(key, data, parent), height(0)
    {}

    signed char getBalance() const
    {
        return ((this->right) ? (this->right->height + 1) : 0) - ((this->left) ? (this->left->height + 1) : 0);
    }

    void fixHeight()
    {
        if(this->right && this->left)
            this->height = max(this->right->height, this->left->height) + 1;
        elif(this->right || this->left)
            this->height = 1;
        else
            this->height = 0;
    }

    void show_node() const
    {
        printf("%d:%d", height, this->key);
    }
};


template<typename TData, typename TKey>
struct avl_node_t : avl_node_crtp<TData, TKey, avl_node_t<TData, TKey>>
{
    avl_node_t(TKey key, TData data, avl_node_t *parent):
            avl_node_crtp<TData, TKey, avl_node_t<TData, TKey>>(key, data, parent)
    {}
};

template<typename TData, typename TKey = int, class avl_node = avl_node_t<TData, TKey>,
        typename enable_if<is_base_of<avl_node_tag, avl_node>::value>::type * = nullptr>
struct avl_tree;

template<typename TData, typename TKey = int, class S = avl_node_t<TData, TKey>>
bool check_avl_tree(avl_tree<TKey, TData, S> *);

template<typename TData, typename TKey, class avl_node,
        typename enable_if<is_base_of<avl_node_tag, avl_node>::value>::type *>
struct avl_tree: search_tree_abstract_crtp<TData, TKey, avl_node>
{
    friend bool check_avl_tree<>(avl_tree<TData, TKey, avl_node> *);

    typedef tree_iterator<TData, TKey> t_iterator;

    avl_tree(): search_tree_abstract_crtp<TData, TKey, avl_node>()
    {}

    std::pair<t_iterator, bool> insert(TKey key, TData data, bool rewrite = false)
    {
        DEBUGLVLMSG(7, "AVL Tree: Inserting new node");
        this->_lock.lock();
        avl_node *node = 0, **next = &this->root;
        for(;;)
        {
            if(*next)
                node = *next;
            else
            {
                balance((*next = new avl_node(key, data, node)));
                t_iterator t = t_iterator(*next);
                this->_lock.unlock();
                return make_pair(t, true);
            }
            if(node->key != key)
                next = node->key > key ? &(node->left) : &(node->right);
            else
            {
                if(rewrite)
                    (*next)->changeData(data, this->root);
                t_iterator t = t_iterator(*next);
                this->_lock.unlock();
                return make_pair(t, false);
            }
        }
    }

    t_iterator findKey(TKey key) const
    {
        DEBUGLVLMSG(7, "AVL Tree: Searching for node");
        this->_lock.lock();
        avl_node *node = this->root;
        while(node != 0)
            if(node->key > key)
                node = node->left;
            elif(node->key < key)
                node = node->right;
            else
            {
                this->_lock.unlock();
                return t_iterator(node);
            }
        t_iterator t = end();
        this->_lock.unlock();
        return t;
    }

    TData erase(t_iterator iterator_)
    {
        DEBUGLVLMSG(7, "AVL Tree: Deleting node");
        this->_lock.lock();
        avl_node *node = (avl_node *) (iterator_.current);
        avl_node *swap = (node->right) ? node->right : node->left;
        if(swap)
        {
            while(swap->left)
                swap = swap->left;
            if(swap->right)
                swap_node_and_list(swap, swap->right);
            swap_node_and_list(node, swap);
        }
        if(!(node->parent))
            this->root = 0;
        elif(node->parent->left == node)
            node->parent->left = 0;
        else
            node->parent->right = 0;
        TData &data = node->data;
        balance(node);
        delete node;
        this->_lock.unlock();
        return data;
    }

    t_iterator begin() const
    {
        this->_lock.lock();
        avl_node *left = this->root;
        if(!left)
        {
            this->_lock.unlock();
            return t_iterator(0, true);
        }
        while(left->left)
            left = left->left;
        t_iterator t = t_iterator(left);
        this->_lock.unlock();
        return t;
    }

    t_iterator end() const
    {
        this->_lock.lock();
        avl_node *right = this->root;
        if(!right)
        {
            this->_lock.unlock();
            return t_iterator(0, true);
        }
        while(right->right)
            right = right->right;
        t_iterator t = t_iterator(right, true);
        this->_lock.unlock();
        return t;
    }

private:
    void balance(avl_node *node)
    {
        if(!(node = node->parent))
            return;
        DEBUGLVLMSG(7, "AVL Tree: Balancing");
        signed char balance = node->getBalance();
        unsigned char height = node->height;
        bool isRoot = (node == this->root);
        if(balance == 2)
        {
            if(node->right->getBalance() >= 0)
                node = smallLeft(node);
            else
                node = bigLeft(node);
        }
        elif(balance == -2)
        {
            if(node->left->getBalance() <= 0)
                node = smallRight(node);
            else
                node = bigRight(node);
        }
        else
            node->fixHeight();
        if(isRoot)
            this->root = node;
        elif(height != node->height)
            this->balance(node);
    }

    void swap_node_and_list(avl_node *node, avl_node *list)
    {
        if(list->parent == node)
        {
            list->parent = node->parent;
            node->parent = list;
            if(node->left == list)
            {
                list->left = node;
                list->right = node->right;
                if(node->right)
                    node->right->parent = list;
            }
            else
            {
                list->right = node;
                list->left = node->left;
                if(node->left)
                    node->left->parent = list;
            }
        }
        else
        {
            if(list->parent->left == list)
                list->parent->left = node;
            else
                list->parent->right = node;
            swap(list->parent, node->parent);
            list->left = node->left;
            list->right = node->right;
            if(node->left)
                node->left->parent = list;
            if(node->right)
                node->right->parent = list;
        }
        list->height = node->height;
        node->height = 0;
        node->left = 0;
        node->right = 0;
        if(list->parent)
        {
            if(list->parent->left == node)
                list->parent->left = list;
            else
                list->parent->right = list;
        }
        else
            this->root = list;
    }

    avl_node *smallLeft(avl_node *node)
    {
        avl_node *p = node, *q = node->right;
        p->right = q->left;
        if(q->left)
            q->left->parent = p;
        q->left = p;
        q->parent = p->parent;
        p->parent = q;
        if(q->parent)
        {
            if(q->parent->key > q->key)
                q->parent->left = q;
            else
                q->parent->right = q;
        }
        p->fixHeight();
        q->fixHeight();
        return q;
    }

    avl_node *smallRight(avl_node *node)
    {
        avl_node *p = node, *q = node->left;
        p->left = q->right;
        if(q->right)
            q->right->parent = p;
        q->right = p;
        q->parent = p->parent;
        p->parent = q;
        if(q->parent)
        {
            if(q->parent->key > q->key)
                q->parent->left = q;
            else
                q->parent->right = q;
        }
        p->fixHeight();
        q->fixHeight();
        return q;
    }

    inline avl_node *bigLeft(avl_node *node)
    {
        return smallLeft(smallRight(node->right)->parent);
    }

    inline avl_node *bigRight(avl_node *node)
    {
        return smallRight(smallLeft(node->left)->parent);
    }
};

template<typename TKey, typename TData, class S>
static int check_avl_balance(avl_node_crtp<TKey, TData, S> *node)
{
    if(!node)
        return 0;
    if(node->getBalance() < -1 || node->getBalance() > 1)
        return -100;
    int hRight = check_avl_balance(node->right);
    if(hRight < 0)
        return -100;
    int hLeft = check_avl_balance(node->left);
    if(hLeft < 0)
        return -100;
    if(node->height != max(hRight, hLeft))
        return -100;
    return max(hRight, hLeft) + 1;
}

template<typename TData, typename TKey, class S>
bool check_avl_tree(avl_tree<TKey, TData, S> *tree)
{
    tree->lock();
    bool ok = (check_avl_balance(tree->root) && check_binary_tree_node(tree->root));
    tree->unlock();
    return ok;
}
