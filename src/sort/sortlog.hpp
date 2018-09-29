#pragma once

#include "../other/defdef.h"
#include "../template/arraymethods.hpp"
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

template<typename T, typename compare_func<T>::type compare = _less<T>>
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

template<typename T, typename compare_func<T>::type compare = _less<T>>
void mergesort_v2(T *start, T *end)
{
    unsigned dif = end - start;
    if(dif < 2)
        return;
    unsigned max_block_size = to2(dif) >> 1, block_size = 2, i = 1;
    T *swap_array = new T[max_block_size];
    for(; i < dif; i += 2)
        if(compare(start[i], start[i - 1]))
            std::swap(start[i - 1], start[i]);
    if((i-- == dif) && compare(start[i], start[i - 1]))
    {
        if(compare(start[i], start[i - 2]))
            std::swap(start[i - 2], start[i]);
        std::swap(start[i - 1], start[i]);
    }
    while(block_size <= max_block_size)
    {
        for(i = block_size;i < dif - block_size;i += block_size * 2)
            if(compare(start[i], start[i - 1]))
            {
                _copy(swap_array, block_size, start + i - block_size);
                merge_two_arrays<T, compare, true>(swap_array, block_size, start + i, block_size, start + i - block_size);
            }
        if(i < dif)
        {
            _copy(swap_array, block_size, start + i - block_size);
            merge_two_arrays<T, compare, true>(swap_array, block_size, start + i, dif - i, start + i - block_size);
        }
        block_size *= 2;
    }
    delete[] swap_array;
}

template<typename T, typename compare_func<T>::type compare = _less<T>>
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
