#pragma once

#include "../other/defdef.h"
#include "../template/commonmethods.hpp"
#include "../template/typemethods.hpp"
#include "../other/rand.h"
#include "sortsquare.hpp"
#include "partition.hpp"

#include <math.h>

template<typename T>
//TODO
void merge_in_place(T *start, unsigned part_len1, unsigned part_len2)
{
    if(part_len1 < 6)
    {
        insertionsort(start, start + part_len1 + part_len2);
        return;
    }
    T *p2 = start + part_len1;
    unsigned blockLen = (unsigned) sqrt(part_len1 + part_len2);
    for(T *p = start + blockLen; p > start; p--);
    for(; p2 != 0; p2--);
    //mergeblocks(start, p2 - blockLen, p2 + partLen - blockLen, blockLen);

    // for partLen < 6 - insertoinSort
    // else:
    // buffer - sqrt(n) of max elements
    // everything to blocks sqrt(n)
    //
    // sort blocks by first element (except buffer)
    // for last (except buffer) to first block merge it using last(buffer):
    // merging blocks a and b=a+1:
    // )swap buffer and a
    // )typicall merge b and a(placed in the end, where buffer was before swapping)
    // )done(buffer now in the end as it was)
    // now we have sorted all array except buffer and buffer in the end
    // let's sort buffer using insertion sort (for example)
    // ???
}

template<typename T, bool (*compare)(T, T) = _less<T>>
void merge_seq_two(T *const __restrict__ start, unsigned part_len1,
                   T *const __restrict__ destination, unsigned part_len2)
{
    unsigned i = 0, i1 = 0, i2 = part_len1;
    while((part_len1) && (part_len2))
    {
        if(compare(start[i1], start[i2]))
        {
            destination[i++] = start[i1++];
            part_len1--;
        } else
        {
            destination[i++] = start[i2++];
            part_len2--;
        }
    }
    while(part_len1--)
        destination[i++] = start[i1++];
    while(part_len2--)
        destination[i++] = start[i2++];
};

template<typename T, bool (*compare)(T, T) = _less<T>,
                bool sort_in_place = false>
void merge_two_arrays(T *first, unsigned part_len1, T *second, unsigned part_len2, T *destination)
{
    while((part_len1) && (part_len2))
    {
        if(compare(*first, *second))
        {
            *(destination++) = *(first++);
            part_len1--;
        } else
        {
            *(destination++) = *(second++);
            part_len2--;
        }
    }
    while(part_len1--)
        *(destination++) = *(first++);
    if(!sort_in_place)
        while(part_len2--)
            *(destination++) = *(second++);
};

template<typename T, bool (*compare)(T, T) = _less<T>>
void mergesort(T *start, T *end)
{
    unsigned dif = end - start;
    T *swap_array = new T[dif];
    for(unsigned i = 1; i < dif; i += 2)
        if(compare(start[i], start[i - 1]))
            std::swap(start[i - 1], start[i]);
    unsigned block_size = 2;
    while(block_size < dif)
    {
        unsigned i = 0, next = block_size * 2;
        while(next <= dif)
        {
            _copy(swap_array, block_size, start + i);
            merge_two_arrays<T, compare, true>(swap_array, block_size, start + i + block_size, block_size, start + i);
            i = next;
            next += block_size * 2;
        }
        if(i + block_size < dif)
        {
            _copy(swap_array, block_size, start + i);
            merge_two_arrays<T, compare, true>(swap_array, block_size, start + i + block_size, dif - i - block_size,
                                               start + i);
        }
        block_size *= 2;
    }
    delete[] swap_array;
}

template<typename T, bool (*compare)(T, T) = _less<T>>
void quicksort(T *start, T *end)
{
    if(end - start < 17)
    {
        insertionsort<T, compare>(start, end);
        return;
    }
    T df = *(start + (end - start) / 2);
    T *sp = partition<T, compare>(start, end, df);
    quicksort<T, compare>(start, sp);
    quicksort<T, compare>(sp, end);
}

//void timsort()
//{}
//
//void heapsort()
//{}
