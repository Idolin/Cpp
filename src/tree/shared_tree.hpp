#pragma once

#include <algorithm>
#include <vector>

using std::make_pair;
using std::vector;

template<typename TKey = int>
struct shared_tree
{
    struct shared_tree_node
    {
        TKey key;
        shared_tree_node *left, *right;
        unsigned direct_links;

        shared_tree_node(TKey key, shared_tree_node *parent) : key(key), left(0), right(0), direct_links(0)
        {
            if(parent)
            {
                if(parent->key > key)
                    parent->left = this;
                else
                    parent->right = this;
            }
        }

        ~shared_tree_node()
        {}

        void deleteNode(bool recursive = false)
        {
            if(!direct_links--)
            {
                if(recursive)
                {
                    if(left)
                        left->deleteNode(true);
                    if(right)
                        right->deleteNode(true);
                }
                delete this;
            }
        }

        shared_tree_node *share(shared_tree_node *parent)
        {
            shared_tree_node *shared = new shared_tree_node(key, parent);
            this->direct_links--;
            if((shared->right = right))
                right->direct_links++;
            if((shared->left = left))
                left->direct_links++;
            return shared;
        }
    };

    // Bidirectional iterator.
    struct iterator
    {
        vector<shared_tree_node *> path_from_root;
        bool is_end;

        iterator(shared_tree_node *root) : is_end(root == 0)
        {
            path_from_root.push_back(root);
        }

        // Элемент на который сейчас ссылается итератор.
        // Разыменование итератора end() неопределено.
        // Разыменование невалидного итератора неопределено.
        TKey const &operator*() const
        {
            return path_from_root.back()->key;
        }

        // Переход к элементу со следующим по величине ключом.
        // Инкремент итератора end() неопределен.
        // Инкремент невалидного итератора неопределен.
        iterator &operator++()
        {
            if(path_from_root.back()->right)
            {
                path_from_root.push_back(path_from_root.back()->right);
                while(path_from_root.back()->left)
                    path_from_root.push_back(path_from_root.back()->left);
                return *this;
            }
            for(int i = path_from_root.size() - 2; i >= 0; i--)
                if(path_from_root[i]->left == path_from_root[i + 1])
                {
                    for(int k = path_from_root.size() - 1; k > i; k--)
                        path_from_root.pop_back();
                    return *this;
                }
            is_end = true;
            return *this;
        }

        iterator operator++(int)
        {
            iterator r = *this;
            ++*this;
            return r;
        }

        // Переход к элементу со следующим по величине ключом.
        // Декремент итератора begin() неопределен.
        // Декремент невалидного итератора неопределен.
        iterator &operator--()
        {
            if(is_end)
            {
                is_end = false;
                return *this;
            }
            if(path_from_root.back()->left)
            {
                path_from_root.push_back(path_from_root.back()->left);
                while(path_from_root.back()->right)
                    path_from_root.push_back(path_from_root.back()->right);
                return *this;
            }
            for(int i = path_from_root.size() - 2; i >= 0; i--)
                if(path_from_root[i]->right == path_from_root[i + 1])
                {
                    for(int k = path_from_root.size() - 1; k > i; k--)
                        path_from_root.pop_back();
                    return *this;
                }
            return *this;
        }

        iterator operator--(int)
        {
            iterator r = *this;
            --*this;
            return r;
        }

        // Сравнение. Итераторы считаются эквивалентными если одни ссылаются на один и тот же элемент.
        // Сравнение с невалидным итератором не определено.
        // Сравнение итераторов двух разных контейнеров не определено.
        bool operator==(const iterator i) const
        {
            return ((is_end == i.is_end) && (path_from_root.back() == i.path_from_root.back()));
        }

        bool operator!=(const iterator i) const
        {
            return !(*this == i);
        }

        shared_tree_node *share()
        {
            bool has_links = false;
            for(unsigned i = 0; i < path_from_root.size(); i++)
            {
                if(!has_links && (path_from_root[i]->direct_links > 0))
                    has_links = true;
                if(has_links)
                    path_from_root[i] = path_from_root[i]->share(i > 0 ? path_from_root[i - 1] : 0);
            }
            return path_from_root[0];
        }

        void followLeft()
        {
            path_from_root.push_back(getNode()->left);
        }

        void followRight()
        {
            path_from_root.push_back(getNode()->right);
        }

        shared_tree_node *getNode()
        {
            return path_from_root.back();
        }
    };

    shared_tree_node *root;

    // Создает пустой shared_tree.
    shared_tree() : root(0)
    {}

    // Создает копию указанного shared_tree.
    shared_tree(shared_tree const &tree)
    {
        if((root = tree.root))
        {
            root->direct_links++;
            //root = root -> share(0);
        }
    }

    // Изменяет this так, чтобы он содержал те же элементы, что и rhs.
    // Инвалидирует все итераторы, принадлежащие shared_tree this, включая end().
    shared_tree &operator=(shared_tree const &rhs)
    {
        if(this != &rhs)
        {
            shared_tree c(rhs);
            swap(*this, c);
        }
        return *this;
    }

    // Деструктор. Вызывается при удалении объектов shared_tree.
    // Инвалидирует все итераторы ссылающиеся на элементы этого shared_tree
    // (включая итераторы ссылающиеся на элементы следующие за последними).
    ~shared_tree()
    {
        if(root)
            root->deleteNode(true);
    }

    void swap(shared_tree &a, shared_tree &b)
    {
        std::swap(a.root, b.root);
    }

    // Поиск элемента.
    // Возвращает итератор на элемент найденный элемент, либо end().
    iterator find(TKey key)
    {
        iterator it = iterator(this->root);
        while(it.getNode() != 0)
            if(it.getNode()->key > key)
                it.followLeft();
            else if(it.getNode()->key < key)
                it.followRight();
            else
                return it;
        return end();
    }

    // Вставка элемента.
    // 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
    //    на уже присутствующий элемент и false.
    // 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
    //    элемент и true.
    // Если вставка произведена, инвалидирует все итераторы, принадлежащие shared_tree this, включая end().
    std::pair<iterator, bool> insert(TKey key)
    {
        if(!root)
            return make_pair(iterator((root = new shared_tree_node(key, 0))), true);
        iterator it = iterator(root);
        shared_tree_node *node;
        for(;;)
        {
            node = it.getNode();
            if(!node)
            {
                it.path_from_root.pop_back();
                root = it.share();
                it.path_from_root.push_back(new shared_tree_node(key, it.getNode()));
                return make_pair(it, true);
            }
            if(node->key != key)
                (node->key > key) ? it.followLeft() : it.followRight();
            else
                return make_pair(it, false);
        }
    }

    // Удаление элемента.
    // Инвалидирует все итераторы, принадлежащие shared_tree this, включая end().
    void erase(iterator it)
    {
        unsigned node_height = it.path_from_root.size() - 1;
        if(it.getNode()->right)
        {
            it.followRight();
            while(it.getNode()->left)
                it.followLeft();
        }
        root = it.share();
        if(root == it.path_from_root[node_height])
        {
            if(root == it.getNode())
                root = (it.getNode()->right) ? it.getNode()->right : it.getNode()->left;
            else
                root = it.getNode();
        }
        if(it.path_from_root.size() > node_height + 1)
        {
            if(node_height == it.path_from_root.size() - 2)
            {
                if(it.path_from_root[node_height]->left == it.getNode())
                {
                    it.path_from_root[node_height]->left = it.getNode()->left;
                    it.getNode()->left = it.path_from_root[node_height];
                    std::swap(it.path_from_root[node_height]->right, it.getNode()->right);
                } else
                {
                    it.path_from_root[node_height]->right = it.getNode()->right;
                    it.getNode()->right = it.path_from_root[node_height];
                    std::swap(it.path_from_root[node_height]->left, it.getNode()->left);
                }
            } else
            {
                if(it.path_from_root[it.path_from_root.size() - 2]->left == it.getNode())
                    it.path_from_root[it.path_from_root.size() - 2]->left = it.path_from_root[node_height];
                else
                    it.path_from_root[it.path_from_root.size() - 2]->right = it.path_from_root[node_height];
                std::swap(it.path_from_root[node_height]->left, it.getNode()->left);
                std::swap(it.path_from_root[node_height]->right, it.getNode()->right);
            }
            if(node_height)
            {
                if(it.path_from_root[node_height - 1]->left == it.path_from_root[node_height])
                    it.path_from_root[node_height - 1]->left = it.getNode();
                else
                    it.path_from_root[node_height - 1]->right = it.getNode();
            }
            shared_tree_node *tmp = it.path_from_root[node_height];
            it.path_from_root[node_height] = it.path_from_root[it.path_from_root.size() - 1];
            it.path_from_root[it.path_from_root.size() - 1] = tmp;
        }
        if(it.path_from_root.size() >= 2)
        {
            shared_tree_node *next = (it.getNode()->right) ? it.getNode()->right : it.getNode()->left;
            if(it.path_from_root[it.path_from_root.size() - 2]->right == it.getNode())
                it.path_from_root[it.path_from_root.size() - 2]->right = next;
            else
                it.path_from_root[it.path_from_root.size() - 2]->left = next;
        }
        it.getNode()->deleteNode();
    }

    // Возващает итератор на элемент с минимальный ключом.
    iterator begin() const
    {
        iterator it = iterator(root);
        if(!root)
            return it;
        while(it.getNode()->left)
            it.followLeft();
        return it;
    }

    // Возващает итератор на элемент следующий за элементом с максимальным ключом.
    iterator end() const
    {
        iterator it = iterator(root);
        if(!root)
            return it;
        while(it.getNode()->right)
            it.followRight();
        return ++it;
    }
};
