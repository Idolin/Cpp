#pragma once

#include "../algo/backpack.hxx"
#include "../struct_algo/convexhull.hxx"
#include "../struct_algo/optimalcode.hxx"
#include "../struct_algo/sequences.hxx"

unsigned long long costTF(unsigned *costCut, unsigned *costFuel, unsigned length)
{
    unsigned long long *minCostCut = new unsigned long long[length];
    minCostCut[0] = 0;
    unsigned long long minCost = 0;
    for(unsigned i = 1; i < length; i++)
    {
        minCost = (unsigned long long) costFuel[0] * costCut[i];
        for(unsigned j = 1; j < i; j++)
            if(minCost > (unsigned long long) minCostCut[j] + (unsigned long long) costFuel[j] * costCut[i])
                minCost = (unsigned long long) minCostCut[j] + (unsigned long long) costFuel[j] * costCut[i];
        minCostCut[i] = minCost;
    }
    delete[] minCostCut;
    return minCost;
}
