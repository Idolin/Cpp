#pragma once

#include <type_traits>
#include <stdint.h>

template<typename T, typename Enable = void>
struct _valueOtherMethods
{};

template<typename T>
struct _valueOtherMethods<T, typename std::enable_if<std::is_integral<T>::value &&
    !std::is_same<T, bool>::value>::type>
{
    constexpr static typename std::make_unsigned<T>::type to_unsigned(T x)
    {
        return static_cast<typename std::make_unsigned<T>::type>(x);
    }

    constexpr static unsigned char getFirstByte(T x)
    {
        return static_cast<unsigned char>(x);
    }

    constexpr static unsigned char getNthByte(T x, unsigned char byte)
    {
        return (byte) ? getNthByte(x >> 8, byte - 1) : getFirstByte(x);
    }

    constexpr static T rol(T x, unsigned char shift)
    {
        shift &= (4 << sizeof(T)) - 1;
        return (x << shift) | (x >> (sizeof(T) * 8 - shift));
    }

    constexpr static T ror(T x, unsigned char shift)
    {
        shift &= (4 << sizeof(T)) - 1;
        return static_cast<T>(to_unsigned(x) >> shift) | (x << (sizeof(T) * 8 - shift));
    }

    typedef typename std::make_unsigned<T>::type to_unsigned_type;
};

template<typename T>
struct _valueOtherMethods<T, typename std::enable_if<std::is_pointer<T>::value &&
        (sizeof(T) == 4)>::type>
{
    constexpr static uint32_t to_unsigned(T x)
    {
        return reinterpret_cast<uint32_t>(x);
    }

    constexpr static unsigned char getFirstByte(T x)
    {
        return static_cast<unsigned char>(to_unsigned(x));
    }

    constexpr static unsigned char getNthByte(T x, unsigned char byte)
    {
        return _valueOtherMethods<uint32_t>::getNthByte(to_unsigned(x), byte);
    }

    typedef uint32_t to_unsigned_type;
};

template<typename T>
struct _valueOtherMethods<T, typename std::enable_if<std::is_floating_point<T>::value &&
        (sizeof(T) == 4)>::type>
{
    constexpr static uint32_t to_unsigned(T x)
    {
        return *reinterpret_cast<uint32_t*>(&x);
    }

    typedef uint32_t to_unsigned_type;
};

template<typename T>
struct _valueOtherMethods<T, typename std::enable_if<std::is_floating_point<T>::value &&
        (sizeof(T) == 8)>::type>
{
    constexpr static uint64_t to_unsigned(T x)
    {
        return *reinterpret_cast<uint64_t*>(&x);
    }

    typedef uint64_t to_unsigned_type;
};

template<typename T>
struct _valueOtherMethods<T, typename std::enable_if<std::is_pointer<T>::value &&
                                                     (sizeof(T) == 8)>::type>
{
    constexpr static uint64_t to_unsigned(T x)
    {
        return reinterpret_cast<uint64_t>(x);
    }

    constexpr static unsigned char getFirstByte(T x)
    {
        return static_cast<unsigned char>(to_unsigned(x));
    }

    constexpr static unsigned char getNthByte(T x, unsigned char byte)
    {
        return _valueOtherMethods<uint64_t>::getNthByte(to_unsigned(x), byte);
    }

    typedef uint64_t to_unsigned_type;
};

template<>
struct _valueOtherMethods<bool>
{
    constexpr static unsigned char to_unsigned(bool x)
    {
        return static_cast<unsigned char>(x);
    }

    constexpr static unsigned char getFirstByte(bool x)
    {
        return static_cast<unsigned char>(x);
    }

    constexpr static unsigned char getNthByte(bool x, unsigned char byte)
    {
        return (byte) ? 0 : to_unsigned(x);
    }

    typedef unsigned char to_unsigned_type;
};


template<typename T, typename Enable = void>
struct _valueMethods;

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 1)>::type>:
        _valueOtherMethods<T>
{
    constexpr static bool bytesEqual(T x)
    {
        return true;
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 2)>::type>:
        _valueOtherMethods<T>
{
    constexpr static bool bytesEqual(T x)
    {
        return (((x & 0x7f00) >> 8) | ((x & 0x8000) ? 0x80 : 0x00)) == (x & 0xff);
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 4) &&
        !std::is_pointer<T>::value>::type>:
        _valueOtherMethods<T>
{
    constexpr static bool bytesEqual(T x)
    {
        return ((((x & 0x7fff0000) >> 16) |
                 ((x & 0x80000000) ? 0x8000 : 0x0000)) == (x & 0xffff)) &&
               (((x & 0x0000ff00) >> 8) == (x & 0xff));
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 8) &&
        !std::is_pointer<T>::value>::type>:
        _valueOtherMethods<T>
{
    constexpr static bool bytesEqual(T x)
    {
        return ((((x & 0x7fffffff00000000) >> 32) |
                 ((x & 0x8000000000000000) ? 0x80000000 :
                  0x00000000)) == (x & 0xffffffff)) &&
                (((x & 0x00000000ffff0000) >> 16) == (x & 0xffff)) &&
                (((x & 0x000000000000ff00) >> 8) == (x & 0xff));
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 4) &&
                                                std::is_pointer<T>::value>::type>:
        _valueOtherMethods<T>
{
    constexpr static bool bytesEqual(T x)
    {
        return _valueMethods<uint32_t>::bytesEqual(reinterpret_cast<uint32_t>(x));
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 8) &&
                                                std::is_pointer<T>::value>::type>:
        _valueOtherMethods<T>
{
    constexpr static bool bytesEqual(T x)
    {
        return _valueMethods<uint64_t>::bytesEqual(reinterpret_cast<uint64_t>(x));
    }
};
