#include "rand.h"

static randomizer defaultRandomizer;

randomizer::randomizer(): state(9316722631499553187LLU)
{}

bool randomizer::randomB()
{
    return static_cast<bool>(next() & 0x01);
}

template<>
bool randomizer::random()
{
    return this->randomB();
}

unsigned char randomizer::randomUC()
{
    return static_cast<unsigned char>(next());
}

template<>
unsigned char randomizer::random()
{
    return this->randomUC();
}

char randomizer::randomC()
{
    return static_cast<char>(next());
}

template<>
char randomizer::random()
{
    return this->randomC();
}

unsigned short randomizer::randomUS()
{
    return static_cast<unsigned short>(next());
}

template<>
unsigned short randomizer::random()
{
    return this->randomUS();
}

short randomizer::randomS()
{
    return static_cast<short>(next());
}

template<>
short randomizer::random()
{
    return this->randomS();
}

unsigned randomizer::randomU()
{
    return static_cast<unsigned>(next());
}

template<>
unsigned randomizer::random()
{
    return this->randomU();
}

int randomizer::randomI()
{
    return static_cast<int>(next());
}

template<>
int randomizer::random()
{
    return this->randomI();
}

unsigned long randomizer::randomUL()
{
    return static_cast<unsigned long>(next());
}

template<>
unsigned long randomizer::random()
{
    return this->randomUL();
}

long randomizer::randomL()
{
    return static_cast<unsigned long>(next());
}

template<>
long randomizer::random()
{
    return this->randomL();
}

unsigned long long randomizer::randomULL()
{
    return static_cast<unsigned long long>(next());
}

template<>
unsigned long long randomizer::random()
{
    return this->randomULL();
}

long long randomizer::randomLL()
{
    return static_cast<long long>(next());
}

template<>
long long randomizer::random()
{
    return this->randomLL();
}

uint8_t randomizer::randomU8()
{
    return static_cast<uint8_t>(next());
}

int8_t randomizer::random8()
{
    return static_cast<int8_t>(next());
}

uint16_t randomizer::randomU16()
{
    return static_cast<uint16_t>(next());
}

int16_t randomizer::random16()
{
    return static_cast<int16_t>(next());
}

uint32_t randomizer::randomU32()
{
    return static_cast<uint32_t>(next());
}

int32_t randomizer::random32()
{
    return static_cast<int32_t>(next());
}


uint64_t randomizer::randomU64()
{
    return static_cast<uint64_t>(next());
}

int64_t randomizer::random64()
{
	return static_cast<int64_t>(next());
}

uint64_t randomizer::next()
{
	state = state * 198277LLU + 3;
    state ^= state << 3;
    state ^= state >> 7;
	return state;
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

template<typename T>
std::enable_if_t<std::is_unsigned<T>::value, T> randomA(T to)
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
