#pragma once

#include "../other/commonmethods.hxx"
#include "../other/rand.h"
#include "sortsquare.hxx"

#include <math.h>

template<typename T>
inline void mergeblocks(T *destination, T *block1, T *block2, unsigned blockLen);

template<typename T>
//TODO
void merge(T *start, unsigned partLen)
{
    if(partLen < 6)
    {
        insertionsort(start, start + partLen * 2);
        return;
    }
    T *p2 = start + partLen;
    unsigned blockLen = (unsigned) sqrt(partLen * 2);
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

template<typename T>
void mergesort(T *start, T *end)
{
    unsigned dif = (unsigned) (end - start);
    if(dif > 3)
    {
        mergesort(start, start + dif / 2);
        mergesort(start + dif / 2, end);
        merge(start, dif / 2);
    } else if(dif > 1)
    {
        end--;
        if(_more(*start, *end))
            _swap(start, end);
        if(dif == 3)
        {
            if(_more(*start, *(start + 1)))
                _swap(start, start + 1);
            else if(_more(*++start, *end))
                _swap(start, end);
        }
    }
}

template<typename T>
void quicksort(T *start, T *end, unsigned count = 0, T min = (T) 0)
{
    if(end - start < 17)
    {
        insertionsort(start, end);
        return;
    }
    T df = *(start + (end - start) / 2);
    T *sp, *esp;
    sp = start + randomU() % (unsigned) (end - start), esp = start + randomU() % (unsigned) (end - start);
    if(_more(*sp, *esp))
    {
        if(_more(*esp, df))
            df = *esp;
        else if(_more(df, *sp))
            df = *sp;
    } else if(_more(*sp, df))
        df = *sp;
    else if(_more(df, *esp))
        df = *esp;
    esp = start + count;
    sp = partition(start, end, df, count, min);
    if(sp > esp + 1)
        quicksort(esp, sp);
    quicksort(sp, end, count, df);
}

void timsort()
{}

void heapsort()
{}
