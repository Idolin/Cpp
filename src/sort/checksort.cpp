#include "checksort.h"

extern unsigned long long swapscounter;

unsigned checkArraySizes[] = {1, 2, 10, 100, 1000, 10000, 100000, 1000000, 1000000};
unsigned sizesToCheck = 9;

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

template<typename T>
void checksortfa(T *s, unsigned mas_len, void(sortf)(T *, T *), double CPCI)
{
    T *e = s + mas_len;
    swapscounter = 0;
    clock_t t = clock();
    sortf(s, e);
    clock_t t2 = clock();
    t2 -= t;
    unsigned long long clocks = t2 - (clock_t) ((double) swapscounter * CPCI + 0.5);
    printf("~%llu clocks, %llu swaps, ", clocks, swapscounter);
    if(_checksorted(s, e))
        printf("OK\n");
    else
        printf("Failed!!!\n");
}

template<typename T>
void checksort(void(sortf)(T *, T *))
{
    double CLOCKSPERCOUNTERINCREASE = getClocksPerCount();
    if(sizesToCheck == 0)
    {
        printf("!!No array sizes to check were given!!");
        return;
    }
    T *t = new unsigned[_max(checkArraySizes, sizesToCheck)];
    for(unsigned sizei = 0; sizei < sizesToCheck; sizei++)
    {
        unsigned size = checkArraySizes[sizei];
        printf("<<Checking size: %u>>\n", size);
        if(size)
        {
            T r = (sizeof(t[0]) <= 32) ? reinterpret_cast<T>(random32()) : reinterpret_cast<T>(random64());
            _vfill(t, size, r);
        }
        printf("\tOne value: ");
        checksortfa(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        for(unsigned i = 0; i < size; i++)
            t[i] = i;
        printf("\tSorted: ");
        checksortfa(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        for(unsigned i = 0; i < size; i++)
            t[i] = size - i;
        printf("\tInversed: ");
        checksortfa(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        if(size >= 10)
        {
            for(unsigned i = 0; i < size; i++)
                t[i] = randomU() & 0x000f;
            printf("/tTen different elements: ");
            checksortfa(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
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
                T startR = (sizeof(t[0]) <= 32) ? reinterpret_cast<T>(random32()) : reinterpret_cast<T>(random64());;
                for(unsigned k = blockStart; k < blockEnd; k++)
                    t[k] = startR += reinterpret_cast<T>(random8());
                blockStart = blockEnd;
            }
            printf("\tPartically sorted: ");
            checksortfa(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        }
        if(sizeof(t[0]) <= 32)
            for(unsigned i = 0; i < size; i++)
                t[i] = reinterpret_cast<T>(random32());
        else
            for(unsigned i = 0; i < size; i++)
                t[i] = reinterpret_cast<T>(random64());
        printf("/tFull random: ");
        checksortfa(t, size, sortf, CLOCKSPERCOUNTERINCREASE);
        usleep(1000);
    }
    delete[] t;
}
