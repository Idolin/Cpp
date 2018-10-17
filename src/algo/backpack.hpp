#pragma once

#include "../template/arraymethods.hpp"

unsigned *backpackW(unsigned backpackCapacity, unsigned *weights, unsigned *values, unsigned &amountOfItems)
{
    unsigned *backpack = new unsigned[++backpackCapacity];
    unsigned *amountToGet = new unsigned[backpackCapacity];
    unsigned *linkNow = new unsigned[backpackCapacity];
    unsigned *linkElmn = new unsigned[backpackCapacity];
    _fill(backpack, backpackCapacity);
    _fill(amountToGet, backpackCapacity);
    _vfill(linkNow, backpackCapacity, backpackCapacity);
    _vfill(linkElmn, backpackCapacity, backpackCapacity);
    for(unsigned i = 0; i < amountOfItems; i++)
        if(weights[i] < backpackCapacity)
        {
            unsigned k = backpackCapacity - weights[i];
            //unsigned ls = 0;
            while(k-- > 0)
                if(backpack[k + weights[i]] < backpack[k] + values[i])
                {
                    backpack[k + weights[i]] = backpack[k] + values[i];
                    amountToGet[k + weights[i]] = amountToGet[k] + 1;
                    if((k >= weights[i]) and (linkNow[k] != backpackCapacity) and (values[linkNow[k]] < values[i]))
                        linkNow[k + weights[i]] = linkNow[k];
                    else
                        linkNow[k + weights[i]] = i;
                }
            //_display(linkNow, backpackCapacity);
        }
    delete[] backpack;
    amountOfItems = amountToGet[--backpackCapacity];
    delete[] amountToGet;
    backpack = new unsigned[amountOfItems];
    unsigned pointer = linkNow[backpackCapacity];
    unsigned weight = backpackCapacity;
    unsigned i = amountOfItems;
    while(i-- > 0)
    {
        backpack[i] = pointer;
        while(pointer == linkNow[weight - weights[pointer]])
            weight--;
        pointer = linkNow[weight -= weights[pointer]];
    }
    delete[] linkNow;
    return backpack;
}

unsigned *backpackNW(unsigned backpackCapacity, unsigned *weights, unsigned *values, unsigned &amount)
{
    unsigned *amountToGet = new unsigned[++backpackCapacity];
    unsigned **backpack = new unsigned *[amount + 1];
    for(unsigned i = 0; i <= amount; i++)
    {
        backpack[i] = new unsigned[backpackCapacity];
        backpack[i][0] = 0;
    }
    _fill(backpack[0], backpackCapacity);
    _fill(amountToGet, backpackCapacity);
    for(unsigned i = 0; i < amount; i++)
        if(weights[i] <= backpackCapacity)
        {
            unsigned k = backpackCapacity - weights[i];
            _copy(backpack[i], weights[i], backpack[i + 1]);
            while(k-- > 0)
                if(backpack[i][k + weights[i]] < backpack[i][k] + values[i])
                {
                    backpack[i + 1][k + weights[i]] = backpack[i][k] + values[i];
                    amountToGet[k + weights[i]] = amountToGet[k] + 1;
                } else
                    backpack[i + 1][k + weights[i]] = backpack[i][k + weights[i]];
        } else
            _copy(backpack[i], backpackCapacity, backpack[i + 1]);
    unsigned w = --backpackCapacity, k = amount;
    unsigned i = amountToGet[backpackCapacity];
    unsigned *backpackItems = new unsigned[i];
    while(backpack[k][w] > 0)
    {
        while(backpack[k - 1][w] == backpack[k][w])
            k--;
        backpackItems[--i] = --k;
        w -= weights[k];
    }
    for(unsigned i = 0; i <= amount; i++)
        delete[] backpack[i];
    delete[] backpack;
    amount = amountToGet[backpackCapacity];
    delete[] amountToGet;
    return backpackItems;
}

