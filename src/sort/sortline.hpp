#pragma once

#include "../other/arraymethods.hxx"
#include "sortsquare.hxx"

template<typename T>
void countsort(T min = 0, T max = 0)
{}

void radixsort(bool right = false, bool nsigned = true);

void bucketsort_u(unsigned *start, unsigned *end, unsigned n = 18)
{
    unsigned dif = (unsigned) (end - start);
    unsigned min, max;
    min = max = *start;
    for(unsigned *s = start + 1; s < end; s++)
        if(*s < min)
            min = *s;
        else if(*s > max)
            max = *s;
    unsigned k = (max - min) / (1 << n) + 1;
    k = 1 << (32 - n);
    unsigned *count = new unsigned[k + 1];
    unsigned *m = new unsigned[dif];
    _fill(count, k);
    for(unsigned *s = start; s < end; s++)
    {
        count[*s >> n]++;
        m[s - start] = *s;
    }
    count[k] = dif;
    for(unsigned i = 1; i < k; i++)
        count[i] += count[i - 1];
    for(unsigned *s = m; s < m + dif; s++)
        start[--count[*s >> n]] = *s;
    delete[] m;
    for(unsigned i = 0; i < k; i++)
        if(count[i] < count[i + 1] - 1)
            insertionsort(start + count[i], start + count[i + 1]);
    delete[] count;
}

void bucketsort_s(int *start, int *end, unsigned n = 18)
{
    unsigned *start_u = (unsigned *) start;
    unsigned *end_u = (unsigned *) end;
    unsigned dif = (unsigned) (end - start);
    int min, max;
    min = max = *start;
    for(int *s = start + 1; s < end; s++)
        if(*s < min)
            min = *s;
        else if(*s > max)
            max = *s;
    unsigned k = (max - min) / (1 << n) + 1;
    k = 1 << (32 - n);
    unsigned *count = new unsigned[k + 1];
    unsigned *m = new unsigned[dif];
    _fill(count, k);
    for(unsigned *s = start_u; s < end_u; s++)
    {
        count[(*s ^ (1 << 31)) >> n]++;
        m[s - start_u] = *s;
    }
    count[k] = dif;
    for(unsigned i = 1; i < k; i++)
        count[i] += count[i - 1];
    for(unsigned *s = m; s < m + dif; s++)
        start_u[--count[(*s ^ (1 << 31)) >> n]] = *s;
    delete[] m;
    for(unsigned i = 0; i < k; i++)
        if(count[i] < count[i + 1] - 1)
            insertionsort(start + count[i], start + count[i + 1]);
    delete[] count;
}
