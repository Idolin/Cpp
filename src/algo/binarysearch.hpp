#pragma once

#include "../template/commonmethods.hpp"

template<typename T>
unsigned binSearchLeftBorder(T *start, T *end, T x)
{
    T *leftBorder = start, *rightBorder = end;
    while(leftBorder < rightBorder)
    {
        T *middle = leftBorder + (rightBorder - leftBorder) / 2;
        if(_more(x, *middle))
            leftBorder = middle + 1;
        else
            rightBorder = middle;
    }
    if((rightBorder == end) or (*rightBorder != x))
        return end - start;
    else
        return rightBorder - start;
}

template<typename T>
unsigned binSearchRightBorder(T *start, T *end, T x)
{
    T *leftBorder = start, *rightBorder = end;
    while(leftBorder < rightBorder)
    {
        T *middle = leftBorder + (rightBorder - leftBorder + 1) / 2;
        if(_more(*middle, x))
            rightBorder = middle - 1;
        else
            leftBorder = middle;
    }
    if((rightBorder == start) or (*rightBorder != x))
        return end - start;
    else
        return rightBorder - start;
}

template<typename T>
unsigned binSearchLowerBound(T *start, T *end, T x)
{
    T *leftBorder = start, *rightBorder = end;
    while(leftBorder < rightBorder)
    {
        T *middle = leftBorder + (rightBorder - leftBorder) / 2;
        if(_more(x, *middle))
            leftBorder = middle + 1;
        else
            rightBorder = middle;
    }
    return (unsigned) (rightBorder - start);
}

template<typename T>
unsigned binSearchLowerBound(unsigned start, unsigned end, T *(get)(unsigned), T x)
{
    unsigned leftBorder = start, rightBorder = end;
    while(leftBorder < rightBorder)
    {
        unsigned middle = leftBorder + (rightBorder - leftBorder) / 2;
        if(_more(x, get(middle)))
            leftBorder = middle + 1;
        else
            rightBorder = middle;
    }
    return rightBorder - start;
}

template<typename T>
unsigned binSearchUpperBound(T *start, T *end, T x)
{
    T *leftBorder = start, *rightBorder = end;
    while(leftBorder < rightBorder)
    {
        T *middle = leftBorder + (rightBorder - leftBorder) / 2;
        if(_notmore(x, *middle))
            leftBorder = middle + 1;
        else
            rightBorder = middle;
    }
    return rightBorder - start;
}

template<typename T>
unsigned binSearchUpperBound(unsigned start, unsigned end, T *(get)(unsigned), T x)
{
    unsigned leftBorder = start, rightBorder = end;
    while(leftBorder < rightBorder)
    {
        unsigned middle = leftBorder + (rightBorder - leftBorder) / 2;
        if(_notmore(x, get(middle)))
            leftBorder = middle + 1;
        else
            rightBorder = middle;
    }
    return rightBorder - start;
}
