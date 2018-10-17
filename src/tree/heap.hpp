#pragma once

#include <algorithm>

using std::swap;

template<typename TData, typename TKey = int>
struct heap
{
    struct heap_element
    {
        TKey key;
        TData data;

        heap_element()
        {}

        heap_element(TKey key, TData data) : key(key), data(data)
        {}
    };

    heap_element *heap_array;
    unsigned heap_size, _array_size;
    bool allow_repeats;

    heap() : heap_array(new heap_element[3]), heap_size(0), _array_size(3), allow_repeats(true)
    {}

    heap(unsigned size) : heap_array(new heap_element[size]), heap_size(0), _array_size(size), allow_repeats(true)
    {}

    bool shiftUp(unsigned node)
    {
        while(node != 0)
        {
            unsigned parent = (node - 1) >> 1;
            if(heap_array[parent].key == heap_array[node].key)
            {
                if(!allow_repeats)
                    remove(node);
                return false;
            }
            if((parent & 0x1) ^ (heap_array[node].key > heap_array[parent].key))
                return true;
            swap(heap_array[node], heap_array[parent]);
        }
        return true;
    }

    void shiftDown(unsigned node)
    {
        unsigned next = node * 2 + 1;
        while(next < heap_size)
        {
            if(next + 1 < heap_size && heap_array[next].key > heap_array[next + 1].key)
                next++;
            if(heap_array[node].key <= heap_array[next].key)
                return;
            swap(heap_array[node], heap_array[next]);
            node = next;
            next = node * 2 + 1;
        }
    }

    void remove(unsigned node)
    {
        swap(heap_array[node], heap_array[--heap_size]);
        shiftDown(node);
    }

    bool insert(TKey key, TData data)
    {
        if(heap_size + 1 == _array_size)
            _resize();
        heap_array[heap_size] = heap_element(key, data);
        return shiftUp(heap_size++);
    }

    TData get_min()
    {
        if(heap_size == 0)
            throw 19;
        TData r = heap_array[0].data;
        remove(0);
        return r;
    }

    void _resize()
    {
        heap_element *n = new heap_element[_array_size * 2 + 1];
        _copy(n, _array_size, heap_array);
        delete[] heap_array;
        heap_array = n;
        _array_size = _array_size * 2 + 1;
    }
};
