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

template<typename T, typename compare_func<T>::type compare = def_less<T>, bool count_inversions = false>
typename std::conditional<count_inversions, unsigned long long, void>::type
mergesort(T *start, T *end)
{
    unsigned dif = end - start;
    if(dif < 2)
        return static_cast<typename std::conditional<count_inversions, unsigned long long, void>::type>(0);
    unsigned long long inversions = 0;
    unsigned max_block_size = to2(dif) >> 1, block_size = 2, i = 1;
    T *swap_array = new T[max_block_size];
    for(; i < dif; i += 2)
        if(compare(start[i], start[i - 1]))
        {
            std::swap(start[i - 1], start[i]);
            if(count_inversions)
                inversions++;
        }
    while(block_size <= max_block_size)
    {
        for(i = block_size;i < dif - block_size;i += block_size * 2)
            if(compare(start[i], start[i - 1]))
            {
                _copy(start + i - block_size, block_size, swap_array);
                if(count_inversions)
                    inversions += merge_two_arrays<T, compare, true, true>(swap_array,
                                                        block_size, start + i, block_size, start + i - block_size);
                else
                    merge_two_arrays<T, compare>(swap_array, block_size, start + i, block_size,
                                                 start + i - block_size);
            }
        if(i < dif)
        {
            _copy(start + i - block_size, block_size, swap_array);
            if(count_inversions)
                inversions += merge_two_arrays<T, compare, true, true>(swap_array,
                                                        block_size, start + i, dif - i, start + i - block_size);
            else
                merge_two_arrays<T, compare, true>(swap_array, block_size, start + i,
                                                   dif - i, start + i - block_size);
        }
        block_size *= 2;
    }
    delete[] swap_array;
    if(count_inversions)
        return static_cast<typename std::conditional<count_inversions, unsigned long long, void>::type>(inversions);
    else
        return static_cast<typename std::conditional<count_inversions, unsigned long long, void>::type>(0);
}

namespace
{
    template<typename T>
    typename def_get_by<T>::type _get_random(const T *start, const T *end)
    {
        return *(start + randomA<unsigned long>(end - start));
    }
}

template<typename T, typename compare_func<T>::type compare = def_less<T>,
    typename def_get_by<T>::type (*get_delim)(const T*, const T*) = _get_random<T>>
void quicksort(T *start, T *end)
{
    if(end - start < 17)
    {
        insertionsort<T, compare>(start, end);
        return;
    }
    typename def_get_by<T>::type df = get_delim(start, end);
    T *sp = partition<T, compare>(start, end, df);
    T *sb = sp;
    while(sb > start && !compare(*--sb, df));
    quicksort<T, compare, get_delim>(start, sb + 1);
    while(sp < end && !compare(df, *sp))
        sp++;
    quicksort<T, compare, get_delim>(sp, end);
}

//void timsort()
//{}
//
//void heapsort()
//{}
