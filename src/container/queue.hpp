#pragma once

#include "../template/displaymethods.hpp"
#include "../other/defdef.h"
#include "persistancequeue.hpp"
#include "stack.hpp"
#include "../other/hash.hpp"
#include "../template/struct_tags.hpp"
#include "../struct/optional.hpp"

#include <algorithm>
#include <cstdio>

using std::pair;

template<typename T, bool enable_hash_if_possible = false>
struct queue: HashableStoredConditional<false, enable_hash_if_possible && is_hashable<T>::value>
{
private:
    struct block
    {
        block *next;
        T *array;

        explicit block(unsigned blocksize): next(nullptr), array(new T[blocksize])
        {}

        ~block()
        {
            delete[] array;
        }
    };

    //blocksize - amount of elements in one block, len - current len of the queue
    //block = [element * blocksize, pointer to next block]
    const unsigned blocksize;
    unsigned long len;

    //pointer to (head - first element, tail - last element, headlast - last element of first block,
    //taillast - last element of last block)
    block *head, *tail;
    unsigned headlast, taillast;

    ConditionalStored<uint64_t, enable_hash_if_possible && is_hashable<T>::value> prev_hash;

public:
    explicit queue(unsigned blocksize = 256):
            blocksize(blocksize), len(0), head(nullptr),
            tail(nullptr), headlast(0), taillast(blocksize), prev_hash(0)
    {
        ASSERT(blocksize > 0);
    }

    ~queue()
    {
        while(head)
        {
            block *to_del = head;
            head = head->next;
            delete to_del;
        }
    }

    void push(typename def_get_by<T>::type x)
    {
        if(enable_hash_if_possible && is_hashable<T>::value)
        {
            uint64_t element_hash = get_hash(x);
            this->set_hash(this->get_hash_value() + (~prev_hash.get_value() ^ element_hash)),
                    this->prev_hash.set_value(element_hash);
        }
        if(taillast == blocksize)
        {
            auto *new_last = new block(blocksize);
            if(tail)
                tail->next = new_last;
            tail = new_last;
            if(!head)
                head = tail;
            taillast = 0;
        }
        tail->array[taillast++] = x;
        len++;
    }

    T&& pop() //a
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
        T &element = head->array[headlast++];
        if(enable_hash_if_possible && is_hashable<T>::value)
        {
            uint64_t next_element_hash = 0;
            if(len)
            {
                if(headlast == blocksize)
                {
                    auto *new_head = head->next;
                    delete head;
                    head = new_head;
                    ASSERT(head);
                    headlast = 0;
                }
                next_element_hash = get_hash(head->array[headlast]);
            }
            this->set_hash(this->get_hash_value() - (~get_hash(element) ^ next_element_hash));
        }
        return std::move(element);
    }

    unsigned long size() const
    {
        return len;
    }

    bool empty() const
    {
        return (size() == 0);
    }

    void display() const
    {
        if(len == 0)
            puts("Queue is empty");
        else
        {
            unsigned block_index = headlast;
            block *b_ptr = head;
            for(unsigned i = 0;i < len;i++)
            {
                if(i > 0)
                    fputs(", ", stdout);
                if(block_index == blocksize)
                {
                    b_ptr = b_ptr->next;
                    block_index = 0;
                }
                _tshow(b_ptr->array[block_index++]);
            }
            putchar('\n');
        }
    }
};

template<typename T, bool enable_hash_if_possible = false>
struct queue_cycled: HashableStoredConditional<false, enable_hash_if_possible && is_hashable<T>::value>
{
private:
    struct block
    {
        T *array;
        // blockmask = 2^x - 1, blockmask > 0(blocksize > 1)
        unsigned head, tail, blockmask;

        explicit block(): array(nullptr), blockmask(0)
        {}

        explicit block(unsigned blocksize): array(new T[blocksize]), head(0), tail(0), blockmask(blocksize - 1)
        {
            ASSERT(is_power_of_2(blocksize));
            ASSERT(blocksize > 1);
        }

        ~block()
        {
            delete[] array;
        }
        
        void init(unsigned blocksize)
        {
            ASSERT(is_power_of_2(blocksize));
            array = new T[blocksize];
            head = tail = 0;
            blockmask = blocksize - 1;
        }
        
        void copy_from(const block& otr)
        {
            ASSERT(blockmask == 0);
			delete[] array;
            ASSERT(this != &otr);
            array = otr.array;
            head = otr.head;
            tail = otr.tail;
            blockmask = otr.blockmask;
        }
        
        unsigned move_to(T *array_to)
        {
			unsigned len;
			if(tail < head)
			{
				len = head - tail;
				_move(array + tail, len, array_to);
			}
			else
			{
				len = blockmask + 1 - tail;
				_move(array + tail, len, array_to);
				_move(array, head, array_to + len);
				len += head;
			}
			delete[] array;
			array = nullptr;
			blockmask = 0;
			return len;
        }
        
        unsigned size() const
		{
			if(tail < head)
				return head - tail;
			return blockmask - (tail - head) + 1;
		}
    };

    unsigned long len;

    //pointer to (head - first element, tail - last element, headlast - last element of first block,
    //taillast - last element of last block)
    block block_new, block_old;

    ConditionalStored<uint64_t, enable_hash_if_possible && is_hashable<T>::value> prev_hash;

public:
    explicit queue_cycled(unsigned blocksize = 256):
            len(0), block_new(_max(to2(blocksize), 2)), block_old(), prev_hash(0)
    {}

    ~queue_cycled() = default;

    void push(typename def_get_by<T>::type x)
    {
        if(enable_hash_if_possible && is_hashable<T>::value)
        {
            uint64_t element_hash = get_hash(x);
            this->set_hash(this->get_hash_value() + (~prev_hash.get_value() ^ element_hash)),
                    this->prev_hash.set_value(element_hash);
        }
        block_new.array[block_new.head++] = x;
        block_new.head &= block_new.blockmask;
        if(block_new.head == block_new.tail)
        {
            if(block_old.blockmask == 0)
            {
                block_old.copy_from(block_new);
                block_new.init(++block_new.blockmask << 1);
            }
            else
            {
				unsigned blocksize = (block_new.blockmask + 1) << 2;
				T *array = new T[blocksize];
				unsigned head = block_old.move_to(array);
				head += block_new.move_to(array + head);
				block_new.array = array;
				block_new.head = head;
				block_new.tail = 0;
				block_new.blockmask = blocksize - 1;
            }
        }
        len++;
    }

    T&& pop()
    {
        ASSERT(len > 0);
        len--;
        T *element;
        if(block_old.blockmask != 0)
        {
			element = block_old.array + block_old.tail++;
			block_old.tail &= block_old.blockmask;
			if(block_old.tail == block_old.head)
				block_old.blockmask = 0;
        }
        else
		{
			element = block_new.array + block_new.tail++;
			block_new.tail &= block_new.blockmask;
		}
        if(enable_hash_if_possible && is_hashable<T>::value)
        {
            uint64_t next_element_hash = 0;
            if(len)
            {
                if(block_old.blockmask != 0)
					next_element_hash = get_hash(block_old.array[block_old.tail]);
				else
					next_element_hash = get_hash(block_new.array[block_new.tail]);
            }
            this->set_hash(this->get_hash_value() - (~get_hash(element) ^ next_element_hash));
        }
        return std::move(*element);
    }

    unsigned long size() const
    {
        return len;
    }

    bool empty() const
    {
        return (size() == 0);
    }

    void display() const
    {
        if(len == 0)
            puts("Queue is empty");
        else
        {
			once<false> started;
			if(block_old.blockmask != 0)
			{
				unsigned tail = block_old.tail;
				do
				{
					if(started)
						fputs(", ", stdout);
					_tshow(block_old.array[tail++]);
					tail &= block_old.blockmask;
				}
				while(tail != block_old.head);
				if(block_old.size() == len)
				{
					putchar('\n');
					return;
				}
			}
            unsigned tail = block_new.tail;
			do
			{
				if(started)
					fputs(", ", stdout);
				_tshow(block_new.array[tail++]);
				tail &= block_new.blockmask;
			}
			while(tail != block_new.head);
            putchar('\n');
        }
    }
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
            if(q[p2].first < q[p].first)
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
                if(q[p2].first > q[p3].first)
                    p3 = p2;
                if((++p2 <= len) and (q[p2].first > q[p3].first))
                    p3 = p2;
            }
            if(q[p3].first > q[p].first)
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
        q[len] = std::make_pair(priority, c);
        linka[posn] = len;
        linkl[len] = posn++;
        this->moveup(len++);
    }

    int popmaxpriority(T &c)
    {
        if(len > 0)
        {
            c = q[0].second;
            int ret = q[0].first;
            linkl[0] = linkl[--len];
            q[0] = q[len];
            this->movedown();
            return ret;
        } else
            return -1;
    }

    int getmaxpriority(T &c)
    {
        if(len > 0)
        {
            c = q[0].second;
            return q[0].first;
        } else
            return -1;
    }

    void setPriority(unsigned n, int prioritydif)
    {
        unsigned pos = linkl[n];
        q[pos] = std::make_pair(q[pos].first + prioritydif, q[pos].second);
        if(prioritydif > 0)
            this->moveup(pos);
        else
            this->movedown(pos);
    }
};
