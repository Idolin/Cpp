#pragma once

#include "../other/arraymethods.hpp"
#include "rangeprepare.hxx"

void showCodeSO(unsigned **cutPoint, unsigned y, unsigned x, char *code, char *codePos)
{
    if(x == y)
    {
        *codePos = 0;
        puts(code);
    } else
    {
        *codePos = '0';
        showCodeSO(cutPoint, y, cutPoint[y][x], code, codePos + 1);
        *codePos = '1';
        showCodeSO(cutPoint, cutPoint[y][x] + 1, x, code, codePos + 1);
    }
}

unsigned getOPCWSO(unsigned *frequency, unsigned alphabetLen)
{
    unsigned size;
    if(alphabetLen == 1)
        size = frequency[0];
    grf<unsigned, unsigned long long> *weight = new grf<unsigned, unsigned long long>(frequency, alphabetLen);
    unsigned long long **minCost = _newArray2d<unsigned long long>(alphabetLen, alphabetLen);
    unsigned **cutPoint = _newArray2d<unsigned>(alphabetLen, alphabetLen);
    for(unsigned i = 0; i < alphabetLen; i++)
    {
        cutPoint[i][i] = i;
        minCost[i][i] = 0;
    }
    for(unsigned i = 1; i < alphabetLen; i++)
        for(unsigned k = 0; k < alphabetLen - i; k++)
        {
            unsigned cutPointT = cutPoint[k][i + k - 1];
            unsigned long long minCostD = minCost[k][cutPointT] + minCost[cutPointT + 1][k + i];
            for(unsigned r = cutPointT + 1; r <= cutPoint[k + 1][i + k]; r++)
            {
                unsigned long long minCostO = minCost[k][r];
                if(r < k + i)
                    minCostO += minCost[r + 1][k + i];
                if(minCostD > minCostO)
                {
                    minCostD = minCostO;
                    cutPointT = r;
                }
            }
            minCost[k][i + k] = minCostD + weight->get(k, k + i);
            cutPoint[k][i + k] = cutPointT;
        }
    size = (unsigned) minCost[0][alphabetLen - 1];
    _deleteArray2d<unsigned long long>(minCost, alphabetLen);
    delete weight;
    char *code = new char[2000];
    showCodeSO(cutPoint, 0, alphabetLen - 1, code, code);
    delete[] code;
    _deleteArray2d<unsigned>(cutPoint, alphabetLen);
    return size;
}
