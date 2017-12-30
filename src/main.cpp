#include <stdio.h>
#include <climits>
#include <cstring>
#include <utility>
#include "sort/checksort.hpp"
#include "sort/sortlog.hpp"

bool cmp(const unsigned &x1, const unsigned &x2)
{
    if((x1 & 0x1) ^ (x2 & 0x1))
        return static_cast<bool>(x1 & 0x1);
    else
        return x1 < x2;
}

int main()
{
//    unsigned n;
//    scanf("%u", &n);
//    std::pair<unsigned, unsigned> *tasks = new std::pair<unsigned, unsigned>[n];
//    for(unsigned i = 0;i < n;i++)
//        scanf("%u %u", &tasks[i].first, &tasks[i].second);
//
    //checksort(quicksort_call<unsigned>);
    checksort<unsigned, cmp>(mergesort<unsigned, cmp>);
    //for(unsigned l = 0;l < 100;l++)
    //{
    unsigned l = 132790;
    unsigned *r = new unsigned[l];
    for(unsigned j = 0; j < 0; j++)
    {
        for(unsigned i = 0; i < l; i++)
            r[i] = randomU();
        mergesort<unsigned>(r, r + l);
    }
    //_display(r, 12);
}