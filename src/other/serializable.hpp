//
// Created by cynder on 24.06.19.
//

#pragma once

#include "../string/str.h"
#include "../template/typemethods.hpp"

#include <type_traits>

struct serialized;

template<typename T>
typename std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, serialized> serialize(T obj)
{
    unsigned char *buffer = new unsigned[sizeof(obj)];
    memcpy(buffer, &obj, sizeof(obj));
    return serialized(buffer, sizeof(obj));
};

template<typename T>
typename std::enable_if_t<std::is_same<typename std::remove_cv<T>::type, const char*>::value ||
        std::is_same<typename std::remove_cv<T>::type, char*>::value, serialized>
serialize(T obj)
{
    size_t length = 0;
    while(obj[length])
        length++;
    unsigned char *buffer = new unsigned char[length];
    memcpy(buffer, obj, length);
    return serialized(buffer, length);
};

template<typename T>
typename std::enable_if_t<std::is_class<T>::value, serialized> serialize(const T &obj)
{
    return obj.serialize();
};

template<typename T>
typename std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, T>
deserialize(unsigned char *buffer, size_t)
{
    T res;
    memcpy(&res, buffer, sizeof(T));
    return res;
};

template<typename T>
typename std::enable_if_t<std::is_same<typename std::remove_cv<T>::type, const char*>::value ||
                          std::is_same<typename std::remove_cv<T>::type, char*>::value, char*>
deserialize(unsigned char *buffer, size_t buffer_length)
{
    char *s = new char[buffer_length + 1];
    _copy(reinterpret_cast<char*>(buffer), buffer_length, s);
    s[buffer_length] = '\0';
    return s;
};

template<typename T>
typename std::enable_if_t<std::is_class<T>::value, T>
deserialize(unsigned char *buffer, size_t buffer_length)
{
    return T::deserialize(buffer, buffer_length);
};

struct serialized
{
    unsigned char *buffer;
    size_t buffer_length;

    serialized(): buffer(nullptr), buffer_length(0)
    {}

    serialized(unsigned char *buffer, size_t buffer_length): buffer(buffer), buffer_length(buffer_length)
    {}

    serialized(serialized &&otr): buffer(otr.buffer), buffer_length(otr.buffer_length)
    {
        delete[] otr.buffer;
        otr.buffer = nullptr;
        otr.buffer_length = 0;
    }

    ~serialized()
    {
        delete[] buffer;
    }

    serialized& operator=(serialized &&otr)
    {
        delete[] buffer;
        buffer = otr.buffer;
        buffer_length = otr.buffer_length;
        otr.buffer = nullptr;
        otr.buffer_length = 0;
        return *this;
    }

    void read(str file, size_t offset = 0, size_t size = to_end)
    {
        FILE *f = fopen(file.c_str(), "rb");
        buffer_length = size;
        if(size == to_end)
        {
            fseek(f, 0, SEEK_END);
            buffer_length = ftell(f) - offset;
        }
        fseek(f, offset, SEEK_SET);
        buffer = new unsigned char[buffer_length];
        fread(buffer, 1, buffer_length, f);
        fclose(f);
    }

    template<typename T>
    void serialize(const T &obj)
    {
        *this = std::move(::serialize(obj));
    }

    template<typename T>
    T deserialize()
    {
        return ::deserialize<T>(buffer, buffer_length);
    }

    unsigned char* data()
    {
        return buffer;
    }

    size_t size()
    {
        return buffer_length;
    }

    static const size_t to_end = std::numeric_limits<size_t>::max();
};