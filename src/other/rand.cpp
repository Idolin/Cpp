#include "rand.h"

static randomizer defaultRandomizer;

randomizer::randomizer() : rand64(UINT64_C(9316722631499553187))
{}

unsigned char randomizer::randomC()
{
    randC ^= randC >> 3;
    randC ^= randC << 7;
    randC ^= randC >> 5;
    return randC;
}

bool randomizer::randomB()
{
    return (randomC() & 0x01);
}

unsigned short randomizer::randomS()
{
    randS ^= randS >> 3;
    randS ^= randS << 7;
    randS ^= randS >> 11;
    return randS;
}

unsigned randomizer::randomU()
{
    randU ^= randU >> 3;
    randU ^= randU << 7;
    randU ^= randU >> 11;
    return randU;
}

unsigned long randomizer::randomL()
{
    randL ^= randL >> 3;
    randL ^= randL << 7;
    randL ^= randL >> 11;
    return randL;
}

unsigned long long randomizer::randomLL()
{
    randLL ^= randLL >> 3;
    randLL ^= randLL << 7;
    randLL ^= randLL >> 11;
    return randLL;
}

uint8_t randomizer::random8()
{
    rand8 ^= rand8 >> 3;
    rand8 ^= rand8 << 7;
    rand8 ^= rand8 >> 5;
    return rand8;
}

uint16_t randomizer::random16()
{
    rand16 ^= rand16 >> 3;
    rand16 ^= rand16 << 7;
    rand16 ^= rand16 >> 11;
    return rand16;
}

uint32_t randomizer::random32()
{
    rand32 ^= rand32 >> 3;
    rand32 ^= rand32 << 7;
    rand32 ^= rand32 >> 11;
    return rand32;
}

uint64_t randomizer::random64()
{
    rand64 ^= rand64 >> 3;
    rand64 ^= rand64 << 7;
    rand64 ^= rand64 >> 11;
    return rand64;
}

bool randomB()
{
    return defaultRandomizer.randomB();
}

unsigned char randomC()
{
    return defaultRandomizer.randomC();
}

unsigned short randomS()
{
    return defaultRandomizer.randomS();
}

unsigned randomU()
{
    return defaultRandomizer.randomU();
}

unsigned long randomL()
{
    return defaultRandomizer.randomL();
}

unsigned long long randomLL()
{
    return defaultRandomizer.randomLL();
}

uint8_t random8()
{
    return defaultRandomizer.random8();
}

uint16_t random16()
{
    return defaultRandomizer.random16();
}

uint32_t random32()
{
    return defaultRandomizer.random32();
}

uint64_t random64()
{
    return defaultRandomizer.random64();
}