#pragma once

#include <cstdint>
#include <type_traits>
#include "../debug/def_debug.h"
#include "../template/commonmethods.hpp"

class randomizer
{
    uint64_t state;
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
	
private:
	uint64_t next();
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

template<typename T>
std::enable_if_t<std::is_unsigned<T>::value, T> randomA(T to);
