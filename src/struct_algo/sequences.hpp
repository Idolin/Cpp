#pragma once

#include "../other/arraymethods.hxx"
#include "../algo/binarysearch.hxx"
#include "../other/commonmethods.hxx"
#include "../sort/sortlog.hxx"

unsigned *
lcs(unsigned *firstSequence, unsigned firstSequenceLen, unsigned *secondSequence, unsigned &sequenceLen) //TODO
{
    unsigned *firstSorted = new unsigned[firstSequenceLen];
    unsigned *indexArray = new unsigned[firstSequenceLen];
    for(unsigned i = 0; i < firstSequenceLen; i++)
        indexArray[i] = i;
    _copy(firstSorted, firstSequenceLen, firstSequence);
    quicksort(firstSorted, firstSorted + firstSequenceLen);
    secondSequence[0] = 0;
    sequenceLen = 0;
    return 0;
}

unsigned *lis(unsigned *sequenceStart, unsigned &sequenceLen)
{
    unsigned *lisa = new unsigned[sequenceLen];
    unsigned *indexLinks = new unsigned[sequenceLen];
    unsigned *lisaIndexLinks = new unsigned[sequenceLen];
    if(sequenceLen == 0)
        return lisa;
    lisa[0] = sequenceStart[0];
    unsigned lisaLen = 1;
    for(unsigned i = 1; i < sequenceLen; i++)
    {
        unsigned insertionIndex = binSearchLowerBound(lisa, lisa + lisaLen, sequenceStart[i]);
        if(insertionIndex > 0)
            indexLinks[i] = lisaIndexLinks[insertionIndex - 1];
        lisaIndexLinks[insertionIndex] = i;
        lisa[insertionIndex] = sequenceStart[i];
        if((insertionIndex ^ lisaLen) == 0)
            lisaLen++;
    }
    unsigned pointer = lisaIndexLinks[lisaLen - 1];
    delete[] lisa;
    delete[] lisaIndexLinks;
    lisa = new unsigned[lisaLen];
    sequenceLen = lisaLen;
    while(lisaLen > 0)
    {
        lisa[--lisaLen] = sequenceStart[pointer];
        pointer = indexLinks[pointer];
    }
    delete[] indexLinks;
    return lisa;
}

template<typename T>
T *lcis(T *firstSequence, unsigned firstSequenceLen, T *secondSequence, unsigned &sequenceLen)
{
    unsigned **lenLCIS = _newArray2d<unsigned>(++firstSequenceLen, ++sequenceLen);
    _fill(lenLCIS[0], sequenceLen);
    unsigned *prev = new unsigned[sequenceLen];
    unsigned bestIndex;
    unsigned bestLength;
    for(unsigned i = 1; i < firstSequenceLen; i++)
    {
        bestIndex = 0;
        bestLength = 1;
        lenLCIS[i][0] = 0;
        for(unsigned k = 1; k < sequenceLen; k++)
        {
            lenLCIS[i][k] = lenLCIS[i - 1][k];
            if((firstSequence[i - 1] == secondSequence[k - 1]) and (lenLCIS[i - 1][k] < bestLength))
            {
                lenLCIS[i][k] = bestLength;
                prev[k] = bestIndex;
            } else if((firstSequence[i - 1] > secondSequence[k - 1]) and (lenLCIS[i - 1][k] >= bestLength))
            {
                bestLength = lenLCIS[i - 1][k] + 1;
                bestIndex = k;
            }
        }
    }
    bestIndex = _maxInd(lenLCIS[--firstSequenceLen], sequenceLen);
    bestLength = lenLCIS[firstSequenceLen][bestIndex];
    unsigned i = bestLength;
    T *lcisa = new T[i];
    while(bestIndex > 0)
    {
        if(i == 0)
            break;
        lcisa[--i] = secondSequence[bestIndex - 1];
        bestIndex = prev[bestIndex];
    }
    _deleteArray2d(lenLCIS, firstSequenceLen + 1);
    delete[] prev;
    sequenceLen = bestLength;
    return lcisa;
}

template<typename T>
unsigned distLevenshtein(T *sequence1, unsigned sequenceLen1, T *sequence2, unsigned sequenceLen2,
                         unsigned insertCost = 1, unsigned removeCost = 1, unsigned changeCost = 1)
{
    unsigned **convCost = new unsigned *[++sequenceLen1];
    sequenceLen2++;
    for(unsigned i = 0; i < sequenceLen1; i++)
    {
        convCost[i] = new unsigned[sequenceLen2];
        convCost[i][0] = i * removeCost;
    }
    for(unsigned i = 0; i < sequenceLen2; i++)
        convCost[0][i] = i * insertCost;
    for(unsigned i = 1; i < sequenceLen1; i++)
        for(unsigned k = 1; k < sequenceLen2; k++)
            if(sequence1[i - 1] != sequence2[k - 1])
            {
                convCost[i][k] = convCost[i - 1][k] + removeCost;
                if(convCost[i][k - 1] + insertCost < convCost[i][k])
                    convCost[i][k] = convCost[i][k - 1] + insertCost;
                if(convCost[i - 1][k - 1] + changeCost < convCost[i][k])
                    convCost[i][k] = convCost[i - 1][k - 1] + changeCost;
            } else
                convCost[i][k] = convCost[i - 1][k - 1];
    unsigned cost = convCost[sequenceLen1 - 1][sequenceLen2 - 1];
    for(unsigned i = 0; i < sequenceLen1; i++)
        delete[] convCost[i];
    delete[] convCost;
    return cost;
}
