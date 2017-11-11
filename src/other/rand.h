#pragma once

#include <stdint.h>

class randomizer
{
    union
    {
        uint8_t rand8;
        uint16_t rand16;
        uint32_t rand32;
        uint64_t rand64;
        unsigned char randC;
        unsigned short randS;
        unsigned randU;
        unsigned long randL;
        unsigned long long randLL;
    };
public:
    randomizer();

    bool randomB();

    unsigned char randomC();

    unsigned short randomS();

    unsigned randomU();

    unsigned long randomL();

    unsigned long long randomLL();

    uint8_t random8();

    uint16_t random16();

    uint32_t random32();

    uint64_t random64();
};

bool randomB();

unsigned char randomC();

unsigned short randomS();

unsigned randomU();

unsigned long randomL();

unsigned long long randomLL();

uint8_t random8();

uint16_t random16();

uint32_t random32();

uint64_t random64();