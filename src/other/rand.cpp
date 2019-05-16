#include "rand.h"

static randomizer defaultRandomizer;

randomizer::randomizer() : randU64(UINT64_C(9316722631499553187))
{}

bool randomizer::randomB()
{
    return static_cast<bool>(randomUC() & 0x01);
}

template<>
bool randomizer::random()
{
    return this->randomB();
}

unsigned char randomizer::randomUC()
{
    randUC ^= randUC >> 3;
    randUC ^= randUC << 7;
    randUC ^= randUC >> 5;
    return randUC;
}

template<>
unsigned char randomizer::random()
{
    return this->randomUC();
}

char randomizer::randomC()
{
    randUC ^= randUC >> 3;
    randUC ^= randUC << 7;
    randUC ^= randUC >> 5;
    return randC;
}

template<>
char randomizer::random()
{
    return this->randomC();
}

unsigned short randomizer::randomUS()
{
    randUS ^= randUS >> 3;
    randUS ^= randUS << 7;
    randUS ^= randUS >> 11;
    return randUS;
}

template<>
unsigned short randomizer::random()
{
    return this->randomUS();
}

short randomizer::randomS()
{
    randUS ^= randUS >> 3;
    randUS ^= randUS << 7;
    randUS ^= randUS >> 11;
    return randS;
}

template<>
short randomizer::random()
{
    return this->randomS();
}

unsigned randomizer::randomU()
{
    randU ^= randU >> 3;
    randU ^= randU << 7;
    randU ^= randU >> 11;
    return randU;
}

template<>
unsigned randomizer::random()
{
    return this->randomU();
}

int randomizer::randomI()
{
    randU ^= randU >> 3;
    randU ^= randU << 7;
    randU ^= randU >> 11;
    return randI;
}

template<>
int randomizer::random()
{
    return this->randomI();
}

unsigned long randomizer::randomUL()
{
    randUL ^= randUL >> 3;
    randUL ^= randUL << 7;
    randUL ^= randUL >> 11;
    return randUL;
}

template<>
unsigned long randomizer::random()
{
    return this->randomUL();
}

long randomizer::randomL()
{
    randUL ^= randUL >> 3;
    randUL ^= randUL << 7;
    randUL ^= randUL >> 11;
    return randL;
}

template<>
long randomizer::random()
{
    return this->randomL();
}

unsigned long long randomizer::randomULL()
{
    randULL ^= randULL >> 3;
    randULL ^= randULL << 7;
    randULL ^= randULL >> 11;
    return randULL;
}

template<>
unsigned long long randomizer::random()
{
    return this->randomULL();
}

long long randomizer::randomLL()
{
    randULL ^= randULL >> 3;
    randULL ^= randULL << 7;
    randULL ^= randULL >> 11;
    return randLL;
}

template<>
long long randomizer::random()
{
    return this->randomLL();
}

uint8_t randomizer::randomU8()
{
    randU8 ^= randU8 >> 3;
    randU8 ^= randU8 << 7;
    randU8 ^= randU8 >> 5;
    return randU8;
}

int8_t randomizer::random8()
{
    randU8 ^= randU8 >> 3;
    randU8 ^= randU8 << 7;
    randU8 ^= randU8 >> 5;
    return rand8;
}

uint16_t randomizer::randomU16()
{
    randU16 ^= randU16 >> 3;
    randU16 ^= randU16 << 7;
    randU16 ^= randU16 >> 11;
    return randU16;
}

int16_t randomizer::random16()
{
    randU16 ^= randU16 >> 3;
    randU16 ^= randU16 << 7;
    randU16 ^= randU16 >> 11;
    return rand16;
}

uint32_t randomizer::randomU32()
{
    randU32 ^= randU32 >> 3;
    randU32 ^= randU32 << 7;
    randU32 ^= randU32 >> 11;
    return randU32;
}

int32_t randomizer::random32()
{
    randU32 ^= randU32 >> 3;
    randU32 ^= randU32 << 7;
    randU32 ^= randU32 >> 11;
    return rand32;
}


uint64_t randomizer::randomU64()
{
    randU64 ^= randU64 >> 3;
    randU64 ^= randU64 << 7;
    randU64 ^= randU64 >> 11;
    return randU64;
}

int64_t randomizer::random64()
{
    randU64 ^= randU64 >> 3;
    randU64 ^= randU64 << 7;
    randU64 ^= randU64 >> 11;
    return rand64;
}

bool randomB()
{
    return defaultRandomizer.randomB();
}

template<>
bool randomA<bool>()
{
    return defaultRandomizer.random<bool>();
}

unsigned char randomUC()
{
    return defaultRandomizer.randomUC();
}

template<>
unsigned char randomA<unsigned char>()
{
    return defaultRandomizer.random<unsigned char>();
}

char randomC()
{
    return defaultRandomizer.randomC();
}

template<>
char randomA<char>()
{
    return defaultRandomizer.random<char>();
}

unsigned short randomUS()
{
    return defaultRandomizer.randomUS();
}

template<>
unsigned short randomA<unsigned short>()
{
    return defaultRandomizer.random<unsigned short>();
}

short randomS()
{
    return defaultRandomizer.randomS();
}

template<>
short randomA<short>()
{
    return defaultRandomizer.random<short>();
}

unsigned randomU()
{
    return defaultRandomizer.randomU();
}

template<>
unsigned randomA<unsigned>()
{
    return defaultRandomizer.random<unsigned>();
}

int randomI()
{
    return defaultRandomizer.randomI();
}

template<>
int randomA<int>()
{
    return defaultRandomizer.random<int>();
}

unsigned long randomUL()
{
    return defaultRandomizer.randomUL();
}

template<>
unsigned long randomA<unsigned long>()
{
    return defaultRandomizer.random<unsigned long>();
}

long randomL()
{
    return defaultRandomizer.randomL();
}

template<>
long randomA<long>()
{
    return defaultRandomizer.random<long>();
}

unsigned long long randomULL()
{
    return defaultRandomizer.randomULL();
}

template<>
unsigned long long randomA<unsigned long long>()
{
    return defaultRandomizer.random<unsigned long long>();
}

long long randomLL()
{
    return defaultRandomizer.randomLL();
}

template<>
long long randomA<long long>()
{
    return defaultRandomizer.random<long long>();
}

uint8_t randomU8()
{
    return defaultRandomizer.randomU8();
}

int8_t random8()
{
    return defaultRandomizer.random8();
}

uint16_t randomU16()
{
    return defaultRandomizer.randomU16();
}

int16_t random16()
{
    return defaultRandomizer.random16();
}

uint32_t randomU32()
{
    return defaultRandomizer.randomU32();
}

int32_t random32()
{
    return defaultRandomizer.random32();
}

uint64_t randomU64()
{
    return defaultRandomizer.randomU64();
}

int64_t random64()
{
    return defaultRandomizer.random64();
}

template<typename T, typename = typename std::enable_if_t<std::is_unsigned<T>::value>>
T randomA(T to)
{
    ASSERT(to > 0);
    T mask = to_bit_mask(to);
    T rand;
    while((rand = randomA<T>() & mask) >= to);
    return rand;
}

template unsigned char randomA<unsigned char>(unsigned char);
template unsigned short randomA<unsigned short>(unsigned short);
template unsigned randomA<unsigned>(unsigned);
template unsigned long randomA<unsigned long>(unsigned long);
template unsigned long long randomA<unsigned long long>(unsigned long long);