#pragma once

#include <algorithm>
#include <stdio.h>
#include <vector>

using std::make_pair;
using std::vector;
using std::swap;

struct persistent_set
{
    typedef int TKey;

    struct tree_node
    {
        TKey key;
        tree_node *left, *right;
        unsigned direct_links;

        tree_node(TKey key, tree_node *parent);

        ~tree_node();

        void deleteNode(bool recursive = false);

        tree_node *share(tree_node *parent);
    };

    // Bidirectional iterator.
    struct iterator
    {
        vector<tree_node *> path_from_root;
        bool is_end;

        iterator(tree_node *root);

        // Элемент на который сейчас ссылается итератор.
        // Разыменование итератора end() неопределено.
        // Разыменование невалидного итератора неопределено.
        TKey const &operator*() const;

        // Переход к элементу со следующим по величине ключом.
        // Инкремент итератора end() неопределен.
        // Инкремент невалидного итератора неопределен.
        iterator &operator++();

        iterator operator++(int);

        // Переход к элементу со следующим по величине ключом.
        // Декремент итератора begin() неопределен.
        // Декремент невалидного итератора неопределен.
        iterator &operator--();

        iterator operator--(int);

        void share();

        void followLeft();

        void followRight();

        tree_node *getNode();
    };

    tree_node *root;

    // Создает пустой shared_avl_tree.
    persistent_set();

    // Создает копию указанного shared_avl_tree.
    persistent_set(persistent_set const &);

    // Изменяет this так, чтобы он содержал те же элементы, что и rhs.
    // Инвалидирует все итераторы, принадлежащие shared_avl_tree this, включая end().
    persistent_set &operator=(persistent_set const &);

    // Деструктор. Вызывается при удалении объектов shared_avl_tree.
    // Инвалидирует все итераторы ссылающиеся на элементы этого shared_avl_tree
    // (включая итераторы ссылающиеся на элементы следующие за последними).
    ~persistent_set();

    // Поиск элемента.
    // Возвращает итератор на элемент найденный элемент, либо end().
    iterator find(TKey);

    // Вставка элемента.
    // 1. Если такой ключ уже присутствует, вставка не производиться, возвращается итератор
    //    на уже присутствующий элемент и false.
    // 2. Если такого ключа ещё нет, производиться вставка, возвращается итератор на созданный
    //    элемент и true.
    // Если вставка произведена, инвалидирует все итераторы, принадлежащие shared_avl_tree this, включая end().
    std::pair<iterator, bool> insert(TKey);

    // Удаление элемента.
    // Инвалидирует все итераторы, принадлежащие shared_avl_tree this, включая end().
    void erase(iterator);

    // Возващает итератор на элемент с минимальный ключом.
    iterator begin() const;

    // Возващает итератор на элемент следующий за элементом с максимальным ключом.
    iterator end() const;
};

// Сравнение. Итераторы считаются эквивалентными если одни ссылаются на один и тот же элемент.
// Сравнение с невалидным итератором не определено.
// Сравнение итераторов двух разных контейнеров не определено.
bool operator==(persistent_set::iterator, persistent_set::iterator);

bool operator!=(persistent_set::iterator, persistent_set::iterator);

void swap(persistent_set &a, persistent_set &b);
