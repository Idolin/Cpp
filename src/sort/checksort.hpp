#pragma once

#define COUNTSWAPS

#include "../other/rand.h"
#include "../template/arraymethods.hpp"

#include <time.h>
#include <unistd.h>
#include <stdio.h>

extern unsigned long long swapscounter;

unsigned checkArraySizes[] = {0, 1, 2, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
unsigned sizesToCheck = 10;

double getClocksPerCount()
{
    unsigned countAmount = 940172481;
    unsigned *t = new unsigned[1024];
    unsigned s = 0;
    clock_t start = clock();
    for(unsigned i = 0; i < countAmount; i++)
    {
        t[i & 1023] = s++;
    }
    clock_t middle = clock();
    for(unsigned i = 0; i < countAmount; i++)
    {
        t[i & 1023] = s;
    }
    clock_t end = clock();
    return (double) (2 * middle - start - end) / countAmount;
}

template<typename T, typename compare_func<T>::type compare = _less<T>>
bool checksortfa(T *s, unsigned mas_len, void(sortf)(T *, T *), double CPCI = 0.00071407856279258859)
{
    T xor_val = 0;
    for(unsigned i = 0; i < mas_len; i++)
        xor_val ^= s[i];
    T *e = s + mas_len;
    swapscounter = 0;
    clock_t t = clock();
    sortf(s, e);
    clock_t t2 = clock();
    t2 -= t;
    unsigned long long clocks = t2 - (clock_t) ((double) swapscounter * CPCI + 0.5);
    printf("~%llu clocks, %llu swaps, ", clocks, swapscounter);
    for(unsigned i = 0; i < mas_len; i++)
        xor_val ^= s[i];
    if((xor_val == 0) && _checksorted<T, compare>(s, e))
    {
        printf("OK\n");
        return true;
    }
    printf("Failed!!!\n");
    return false;
}

template<typename T, typename compare_func<T>::type compare = _less<T>>
void checksort(void(sortf)(T*, T*))
{
    double CLOCKSPERCOUNTERINCREASE = getClocksPerCount();
    T *t = new T[_max(checkArraySizes, sizesToCheck)];
    for(unsigned sizei = 0; sizei < sizesToCheck; sizei++)
    {
        unsigned size = checkArraySizes[sizei];
        printf("<<Checking size: %u>>\n", size);
        if(size)
        {
            T r = randomA<T>();
            _vfill(t, size, r);
        }
        printf("\tOne value: ");
        checksortfa<T, compare>(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        if(sizeof(T) >= 4)
            for(unsigned i = 0; i < size; i++)
                t[i] = i;
        else
            for(unsigned i = 0; i < size; i++)
                t[i] = (i * 63) << max_bit_pos(size);
        printf("\tSorted: ");
        checksortfa<T, compare>(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        if(sizeof(T) >= 4)
            for(unsigned i = 0; i < size; i++)
                t[i] = size - i;
        else
            for(unsigned i = 0; i < size; i++)
                t[i] = ((size - i) * 63) << max_bit_pos(size);
        printf("\tInverted: ");
        checksortfa<T, compare>(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        if(size >= 10)
        {
            for(unsigned i = 0; i < size; i++)
                t[i] = randomA<T>() & 0x000f;
            printf("\tSixteen different elements: ");
            checksortfa<T, compare>(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        }
        if(size >= 100)
        {
            unsigned halfBlockSize = size / 20;
            unsigned blockStart = 0;
            for(unsigned i = 1; i <= 10; i++)
            {
                unsigned blockEnd;
                if(i == 10)
                    blockEnd = size;
                else
                    blockEnd = halfBlockSize * (2 * i + 1) - randomU() % (halfBlockSize * 2);
                T startR = randomA<T>();
                for(unsigned k = blockStart; k < blockEnd; k++)
                    t[k] = startR += randomA<T>() & 0xff;
                blockStart = blockEnd;
            }
            printf("\tPartically sorted: ");
            checksortfa<T, compare>(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        }
        for(unsigned i = 0; i < size; i++)
            t[i] = randomA<T>();
        printf("\tFull random: ");
        checksortfa<T, compare>(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
    }
    delete[] t;
}
