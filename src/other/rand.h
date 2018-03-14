#pragma once

#include <cstdint>

class randomizer
{
    union
    {
        uint8_t randU8;
        int8_t rand8;
        uint16_t randU16;
        int16_t rand16;
        uint32_t randU32;
        int32_t rand32;
        uint64_t randU64;
        int64_t rand64;
        unsigned char randUC;
        char randC;
        unsigned short randUS;
        short randS;
        unsigned randU;
        int randI;
        unsigned long randUL;
        long randL;
        unsigned long long randULL;
        long long randLL;
    };
public:
    randomizer();
    bool randomB();

    unsigned char randomUC();

    char randomC();

    unsigned short randomUS();

    short randomS();
    unsigned randomU();

    int randomI();

    unsigned long randomUL();

    long randomL();

    unsigned long long randomULL();

    long long randomLL();

    uint8_t randomU8();

    int8_t random8();

    uint16_t randomU16();

    int16_t random16();

    uint32_t randomU32();

    int32_t random32();

    uint64_t randomU64();

    int64_t random64();

    template<typename T>
    T random();
};

bool randomB();

unsigned char randomUC();

char randomC();

unsigned short randomUS();

short randomS();
unsigned randomU();

int randomI();

unsigned long randomUL();

long randomL();

unsigned long long randomULL();

long long randomLL();

uint8_t randomU8();

int8_t random8();

uint16_t randomU16();

int16_t random16();

uint32_t randomU32();

int32_t random32();

uint64_t randomU64();

int64_t random64();

template<typename T>
T randomA();