#pragma once

template<typename T, bool bidirectional = true>
struct linkedList
{
};

template<typename T>
struct linkedList<T, false>
{
    struct node
    {
        T data; //data
        node *next; //pointer to next node

        explicit node(T data, node *next = nullptr) : data(data), next(next)
        {}
        ~node() = default;
    };

    node *head, *tail;
    unsigned len;

    linkedList() : head(nullptr), tail(nullptr), len(0)
    {}

    ~linkedList()
    {
        node *it = head, *next;
        while(it)
        {
            next = it->next;
            delete it;
            it = next;
        }
    }

    node *insert(node *now, T data) //insert new node after now
    {
        len++;
        now->next = new node(data, now->next);
        if(tail == now)
            tail = now->next;
        return now->next;
    }

    node *push_front(T data) //insert new node to the front
    {
        len++;
        if(head == nullptr)
            return (head = tail = new node(data));
        else
            return (head = new node(data, head));
    }

    node *push_tail(T data) //insert new node to the tail
    {
        len++;
        if(tail == nullptr)
            return (head = tail = new node(data));
        else
        {
            tail->next = new node(data);
            return (tail = tail->next);
        }
    }

    T remove_next(node *prev) //remove node after prev (head for prev == nullptr)
    {
        if(!prev)
            return remove_head();
        ASSERT(prev != tail);
        len--;
        node *next = prev->next;
        if(next == tail)
            tail = prev;
        T data = next->data;
        prev->next = next->next;
        delete next;
        return data;
    }

    T remove_head() //remove first node and return it's data
    {
        ASSERT(head);
        len--;
        T data = head->data;
        node *second = head->next;
        delete head;
        head = second;
        if(!head)
            tail = nullptr;
        return data;
    }

    node *at(unsigned index) //return node at index position
    {
        ASSERT(index < len);
        node *it = head;
        while(index--)
            it = it->next;
        return it;
    }

    bool empty() //if list is empty
    {
        return (len == 0);
    }
};

template<typename T>
struct linkedList<T, true>
{
    struct node
    {
        T data;
        node *next, *prev;

        explicit node(T data, node *next = nullptr, node *prev = nullptr) : data(data), next(next), prev(prev)
        {}

        ~node() = default;
    };

    node *head, *tail;
    unsigned len;

    linkedList() : head(nullptr), tail(nullptr), len(0)
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
            delete tail;
        }
    }

    node *insert(node *now, T data) //insert new node after now
    {
        len++;
        now->next = new node(data, now->next, now);
        if((now = now->next)->next)
            now->next->prev = now;
        else
            tail = now;
    }

    node *push_front(T data)
    {
        len++;
        if(head)
        {
            head->prev = new node(data, head, nullptr);
            return (head = head->prev);
        }
        else
            return (head = tail = new node(data));
    }

    node *push_tail(T data)
    {
        len++;
        if(tail)
        {
            tail->next = new node(data, nullptr, tail);
            return (tail = tail->next);
        }
        else
            return (head = tail = new node(data));
    }

    T remove(node *rem)
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
        delete rem;
        return data;
    }

    T remove_head()
    {
        ASSERT(head);
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
            delete head;
            head = tail = nullptr;
        }
        return data;
    }

    T remove_tail()
    {
        ASSERT(tail);
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
            delete tail;
            head = tail = nullptr;
        }
        return data;
    }

    node *at(unsigned index)
    {
        ASSERT(index < len);
        node *it;
        if(index < len / 2)
        {
            it = head;
            while(index--)
                it = it->next;
        }
        else
        {
            it = tail;
            while(++index < len)
                it = it->prev;
        }
        return it;
    }

    bool empty()
    {
        return (len == 0);
    }
};
