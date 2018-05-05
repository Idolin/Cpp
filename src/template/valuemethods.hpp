#pragma once

#include <type_traits>

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
};

template<typename T>
struct _valueOtherMethods<T, typename std::enable_if<std::is_pointer<T>::value &&
        (sizeof(T) == 4)>::type>
{
    constexpr static u_int32_t to_unsigned(T x)
    {
        return reinterpret_cast<u_int32_t>(x);
    }

    constexpr static unsigned char getFirstByte(T x)
    {
        return static_cast<unsigned char>(to_unsigned(x));
    }
};

template<typename T>
struct _valueOtherMethods<T, typename std::enable_if<std::is_pointer<T>::value &&
                                                     (sizeof(T) == 8)>::type>
{
    constexpr static u_int64_t to_unsigned(T x)
    {
        return reinterpret_cast<u_int64_t>(x);
    }

    constexpr static unsigned char getFirstByte(T x)
    {
        return static_cast<unsigned char>(to_unsigned(x));
    }
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
        return _valueMethods<u_int32_t>::bytesEqual(reinterpret_cast<u_int32_t>(x));
    }
};

template<typename T>
struct _valueMethods<T, typename std::enable_if<(sizeof(T) == 8) &&
                                                std::is_pointer<T>::value>::type>:
        _valueOtherMethods<T>
{
    constexpr static bool bytesEqual(T x)
    {
        return _valueMethods<u_int64_t>::bytesEqual(reinterpret_cast<u_int64_t>(x));
    }
};