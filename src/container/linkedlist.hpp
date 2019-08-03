#pragma once

#include "../debug/def_debug.h"
#include <iterator>

template<typename T, bool bidirectional = true>
struct linkedList
{};

template<typename T>
struct linkedList<T, false>
{
private:
    struct node
    {
        T data; //data
        node *next; //pointer to next node

        explicit node(const T &data, node *next = nullptr): data(data), next(next)
        {}
        ~node() = default;
    };

public:
    struct iterator: public std::iterator<std::forward_iterator_tag, T>
    {
        friend struct linkedList<T, false>;

    private:
        node *ptr;

    public:

        iterator(node *ptr = nullptr): ptr(ptr)
        {}

        iterator(const iterator& otr): ptr(otr.ptr)
        {}

        iterator& operator=(const iterator& otr)
        {
            ptr = otr.ptr;
            return *this;
        }

        bool operator==(const iterator& otr) const
        {
            return ptr == otr.ptr;
        }

        bool operator!=(const iterator& otr) const
        {
            return !(*this == otr);
        }

        iterator& operator++()
        {
            ptr = ptr->next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator copy = *this;
            ptr = ptr->next;
            return copy;
        }

        T& operator*() const
        {
            return ptr->data;
        }

        node *operator->() const
        {
            return ptr;
        }
    };

    iterator head, tail;
    unsigned len;

    linkedList(): head(), tail(), len(0)
    {}

    ~linkedList()
    {
        node *it = head.ptr, *next;
        while(it)
        {
            next = it->next;
            delete it;
            it = next;
        }
    }

    iterator insert(const iterator &now, const T &data) //insert new node after now
    {
        len++;
        now->next = new node(data, now->next);
        if(tail == now)
            tail = now->next;
        return now->next;
    }

    iterator push_front(const T &data) //insert new node to the front
    {
        len++;
        if(head.ptr == nullptr)
            return (head = tail = new node(data));
        else
            return (head = new node(data, head.ptr));
    }

    iterator push_tail(const T &data) //insert new node to the tail
    {
        len++;
        if(tail.ptr == nullptr)
            return (head = tail = new node(data));
        else
        {
            tail->next = new node(data);
            return (tail = tail->next);
        }
    }

    T remove_next(const iterator &prev) //remove node after prev (head for prev == end())
    {
        if(!prev.ptr)
            return remove_head();
        ASSERT(prev != tail);
        len--;
        node *next = prev->next;
        if(next == tail.ptr)
            tail = prev;
        T data = next->data;
        prev->next = next->next;
        delete next;
        return data;
    }

    T remove_head() //remove first node and return it's data
    {
        ASSERT(head.ptr);
        len--;
        T data = head->data;
        node *second = head->next;
        delete head.ptr;
        head = second;
        if(!head.ptr)
            tail = nullptr;
        return data;
    }

    iterator at(unsigned index) const //return node at index position
    {
        ASSERT(index < len);
        iterator it = head;
        while(index--)
            it++;
        return it;
    }

    bool empty() const //if list is empty
    {
        return (len == 0);
    }

    iterator begin() const
    {
        return head;
    }

    iterator end() const
    {
        return iterator();
    }
};

template<typename T>
struct linkedList<T, true>
{
private:
    struct node
    {
        T data;
        node *next, *prev;

        explicit node(T data, node *next = nullptr, node *prev = nullptr): data(data), next(next), prev(prev)
        {}

        ~node() = default;
    };

public:
    struct iterator: public std::iterator<std::bidirectional_iterator_tag, T>
    {
        friend struct linkedList<T, true>;

    private:
        node *ptr;
        bool end;

        iterator(node *ptr, bool end): ptr(ptr), end(end)
        {}

    public:
        iterator(): ptr(nullptr), end(true)
        {}

        iterator(node *ptr): ptr(ptr), end(false)
        {}

        iterator(const iterator& otr): ptr(otr.ptr), end(otr.end)
        {}

        iterator& operator=(iterator const &otr)
        {
            ptr = otr.ptr;
            end = otr.end;
            return *this;
        }

        bool operator==(const iterator &otr) const
        {
            if(end ^ otr.end)
                return false;
            return (ptr == otr.ptr) || (end && otr.end);
        }

        bool operator!=(const iterator &otr) const
        {
            return !(*this == otr);
        }

        iterator& operator++()
        {
            if(ptr->next)
                ptr = ptr->next;
            else
                end = true;
            return *this;
        }

        iterator operator++(int)
        {
            iterator copy = *this;
            ++*this;
            return copy;
        }

        iterator& operator--()
        {
            if(end)
                end = false;
            else
                ptr = ptr->prev;
            return *this;
        }

        iterator operator--(int)
        {
            iterator copy = *this;
            --*this;
            return copy;
        }

        T& operator*() const
        {
            return ptr->data;
        }

        node *operator->() const
        {
            return ptr;
        }
    };

    iterator head, tail;
    unsigned len;

    linkedList(): head(), tail(), len(0)
    {}

    ~linkedList()
    {
        if(len)
        {
            node *it = head->next;
            while(it)
            {
                delete it->prev;
                it = it->next;
            }
            delete tail.ptr;
        }
    }

    iterator insert(const iterator &now, const T &data) //insert new node after now
    {
        len++;
        node *tmp = new node(data, now->next, now.ptr);
        if(now->next)
            now->next->prev = tmp;
        else
            tail = tmp;
        now->next = tmp;
        return now->next;
    }

    iterator push_front(const T &data)
    {
        len++;
        if(head.ptr)
        {
            head->prev = new node(data, head.ptr, nullptr);
            return (head = head->prev);
        }
        else
            return (head = tail = new node(data));
    }

    iterator push_tail(const T &data)
    {
        len++;
        if(tail.ptr)
        {
            tail->next = new node(data, nullptr, tail.ptr);
            return (tail = tail->next);
        }
        else
            return (head = tail = new node(data));
    }

    T remove(iterator rem)
    {
        len--;
        if(rem->prev)
            rem->prev->next = rem->next;
        else
            head = rem->next;
        if(rem->next)
            rem->next->prev = rem->prev;
        else
            tail = rem->prev;
        T data = rem->data;
        delete rem.ptr;
        return data;
    }

    T remove_head()
    {
        ASSERT(head.ptr);
        len--;
        T data = head->data;
        if(head->next)
        {
            head = head->next;
            delete head->prev;
            head->prev = nullptr;
        }
        else
        {
            delete head.ptr;
            head = tail = iterator();
        }
        return data;
    }

    T remove_tail()
    {
        ASSERT(tail.ptr);
        len--;
        T data = tail->data;
        if(tail->prev)
        {
            tail = tail->prev;
            delete tail->next;
            tail->next = nullptr;
        }
        else
        {
            delete tail.ptr;
            head = tail = iterator();
        }
        return data;
    }

    iterator at(unsigned index) const
    {
        ASSERT(index < len);
        iterator it;
        if(index < len / 2)
        {
            it = head;
            while(index--)
                it++;
        }
        else
        {
            it = tail;
            while(++index < len)
                it--;
        }
        return it;
    }

    bool empty() const
    {
        return (len == 0);
    }

    iterator begin() const
    {
        return head;
    }

    iterator end() const
    {
        return iterator(tail.ptr, true);
    }
};
