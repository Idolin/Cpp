#pragma once

#include "../other/rand.h"
#include "../template/arraymethods.hpp"
#include "../template/displaymethods.hpp"
#include "../other/counter.h"

#include <time.h>
#include <unistd.h>
#include <stdio.h>

extern unsigned long long swapscounter;

static unsigned checkArraySizes[] = {0, 1, 2, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
static unsigned sizesToCheck = 10;
static unsigned long long total_ms;

template<typename T, typename compare_func<T>::type compare = _less<T>, typename R>
bool checksortfa(T *s, unsigned mas_len, R(*sortf)(T *, T *))
{
    T *copy;
    if(mas_len < 100)
        copy = _new_copy(s, mas_len);
    T xor_val = 0;
    for(unsigned i = 0; i < mas_len; i++)
        xor_val ^= s[i];
    T *e = s + mas_len;
    process_time_counter t;
    t.start();
    sortf(s, e);
    t.stop();
    total_ms += t.getMilliseconds();
    printf("~%llu ms, ", t.getMilliseconds());
    for(unsigned i = 0; i < mas_len; i++)
        xor_val ^= s[i];
    if((xor_val == 0) && _checksorted<T, compare>(s, e))
    {
        printf("OK\n");
        return true;
    }
    printf("Failed!!!");
    if(mas_len < 100)
    {
        fputs(" {", stdout);
        _display(copy, mas_len, _typeSeq<T>::specifier, ",", "} -> {");
        _display(s, mas_len, _typeSeq<T>::specifier, ",", "}");
    }
    putchar('\n');
    return false;
}

template<typename T=int, typename compare_func<T>::type compare = _less<T>, typename R=void>
void checksort(R(*sortf)(T*, T*))
{
    total_ms = 0;
    bool ok = true;
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
        ok &= checksortfa<T, compare, R>(t, size, sortf);
        if(sizeof(T) >= 4)
            for(unsigned i = 0; i < size; i++)
                t[i] = i;
        else
            for(unsigned i = 0; i < size; i++)
                t[i] = (i * 63) << max_bit_pos(size);
        printf("\tSorted: ");
        ok &= checksortfa<T, compare, R>(t, size, sortf);
        if(sizeof(T) >= 4)
            for(unsigned i = 0; i < size; i++)
                t[i] = size - i;
        else
            for(unsigned i = 0; i < size; i++)
                t[i] = ((size - i) * 63) << max_bit_pos(size);
        printf("\tInverted: ");
        ok &= checksortfa<T, compare, R>(t, size, sortf);
        if(size >= 10)
        {
            for(unsigned i = 0; i < size; i++)
                t[i] = randomA<T>() & 0x000f;
            printf("\tSixteen different elements: ");
            ok &= checksortfa<T, compare>(t, size, sortf);
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
            printf("\tParticipially sorted: ");
            ok &= checksortfa<T, compare, R>(t, size, sortf);
        }
        for(unsigned i = 0; i < size; i++)
            t[i] = randomA<T>();
        printf("\tFull random: ");
        ok &= checksortfa<T, compare, R>(t, size, sortf);
    }
    delete[] t;
    printf("Result: ~%llu ms, %s\n", total_ms, ok ? "OK" : "Failed!!!");
}
