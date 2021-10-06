#pragma once

#include <stdexcept>

#if !defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)
    // taken from https://stackoverflow.com/a/27054190/9564477
    #if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
        defined(__BIG_ENDIAN__) || \
        defined(__ARMEB__) || \
        defined(__THUMBEB__) || \
        defined(__AARCH64EB__) || \
        defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)

        #define BIG_ENDIAN

    #elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
        defined(__LITTLE_ENDIAN__) || \
        defined(__ARMEL__) || \
        defined(__THUMBEL__) || \
        defined(__AARCH64EL__) || \
        defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)

        #define LITTLE_ENDIAN

    #else
        #error "Unknown know what architecture this is!"
    #endif
#endif

enum class ByteOrder
{
    LE = 0,
    little_endian = 0,
    BE = 1,
    big_endian = 1,
    native_endianness =
#ifdef LITTLE_ENDIAN
    little_endian,
#elif BIG_ENDIAN
    big_endian,
#else
    #error "No endianness specicfied"
#endif
};


struct CppVersion
{
private:
    int cpp_version;

public:
    constexpr CppVersion(long version = __cplusplus): cpp_version(1997)
    {
        if((version <= 0) ||
                (version >= 100 && version < 1983) ||
                (version > 2020 && version < 198300L) ||
                (version >= 202100L))
            throw std::domain_error("Unknown cpp version number: ");

        if(version < 3)
            version = 83;
        if(version > 100 && version < 198399L)
            version %= 100;
        else
            version = (version / 100) % 100;

        if(version < 11)
            cpp_version = 2003;
        else if(version < 14)
            cpp_version = 2011;
        else if(version < 17)
            cpp_version = 2014;
        else if(version < 20)
            cpp_version = 2017;
        else if(version < 83)
            cpp_version = 2020;
        else if(version < 98)
            cpp_version = 1983;
        else
            version = 1998;
    }

    constexpr bool operator==(CppVersion otr) const
    {
        return cpp_version == otr.cpp_version;
    }

    constexpr bool operator!=(CppVersion otr) const
    {
        return cpp_version != otr.cpp_version;
    }

    constexpr bool operator<(CppVersion otr) const
    {
        return cpp_version < otr.cpp_version;
    }

    constexpr bool operator<=(CppVersion otr) const
    {
        return cpp_version <= otr.cpp_version;
    }

    constexpr bool operator>(CppVersion otr) const
    {
        return cpp_version > otr.cpp_version;
    }

    constexpr bool operator>=(CppVersion otr) const
    {
        return cpp_version >= otr.cpp_version;
    }

    constexpr int get() const
    {
        return cpp_version;
    }
};

constexpr CppVersion cpp_std_version = CppVersion();
