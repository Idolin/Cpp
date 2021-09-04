//
// Created by cynder on 12.02.2021.
//

#pragma once

#include "../container/vector.hpp"
#include "../other/hash.hpp"
#include "../other/endianness.h"

#include <iostream>
#include <string>
#include <type_traits>
#include <uchar.h>

struct str: Hashable
{
    enum byte_order
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

    enum unicode_normalization_form
    {
        NFD = 0,
        canonical_decomposition = 0,
        NFC = 1,
        canonical_composition = 1,
        NFKD = 2,
        compatibility_decomposition = 2,
        NFKC = 3,
        compatibility_composition = 2
    };

    using meta_type = union
        {
            void *ptr;
            std::size_t val;
        };

    struct str_info: HashableStored<true>
    {
        union
        {
            char *block8;
            char16_t *block16;
            char32_t *block32;
        };

        std::size_t len, links;
        meta_type *meta_info;
        char flags;
        char[3] offsets;
    };

    str();

    str(const char*); // assumes UTF-8

    /*
     *  if BOM (byte order mark) will be presented then given byte_order will be ignored
     * and text will be encoded according to presenetd BOM
    */

    str(const char16_t*, byte_order = byte_order::native_endianness); // assumes UTF-16

    str(const char32_t*, byte_order = byte_order::native_endianness); // asuumes UTF-32

    /*
     * if sizeof(wchar_t) == 1: assumes UTF-8
     * if sizeof(wchar_t) == 2: assumes UTF-16
     * otherwise: assumes UTF-32
    */
    str(const wchar_t*);

    str(const std::string&); // assumes UTF-8

    str(std::string&&); // assumes UTF-8

    /*
     * if sizeof(wchar_t) == 1: assumes UTF-8
     * if sizeof(wchar_t) == 2: assumes UTF-16
     * otherwise: assumes UTF-32
    */
    str(const std::wstring&);

    /*
     * if sizeof(wchar_t) == 1: assumes UTF-8
     * if sizeof(wchar_t) == 2: assumes UTF-16
     * otherwise: assumes UTF-32
    */
    str(std::wstring&&);

    str(const str&);

    str(str&&);

    str(bool);

    str(char);

    str(char16_t);

    str(char32_t);

    str(wchar_t);

    template<typename I, std::enable_if_t<std::is_integral<I>::value &&
             !std::is_same<I, char>::value && !std::is_same<I, char16_t>::value &&
             !std::is_same<I, char32_t>::value && !std::is_same<I, wchar_t>::value &&
             !std::is_same<I, bool>::value>>
    str(I);

    str(double);

    ~str();

    bool is_valid() const;

    str& normalize(unicode_normalization_form);

    str& operator=(const str&);

    str& operator=(str&&);

    str& operator+=(char32_t);

    str& operator+=(const str&);

    str& append(const str&); // exactly the same as operator+=

    str& operator*=(unsigned);

    str operator+(const str&) const;

    char32_t operator[](size_t) const;

    char32_t at(size_t) const;

    str& set_char(size_t, char32_t);

    /*
     *  All comparison & search operators will compare strings
     * based on the raw code point values.
     *  It will not take unicode collation into consideration.
    */

    bool operator==(const str&) const;

    bool operator!=(const str&) const;

    bool operator>(const str&) const;

    bool operator>=(const str&) const;

    bool operator<(const str&) const;

    bool operator<=(const str&) const;

    const char *raw_data() const;

    const char *c_str() const; // exactly the same as raw_data

    char* as_utf8() const;

    char16_t* as_utf16() const;

    char32_t* as_utf32() const;

    operator std::string() const;

    operator std::wstring() const;

    str copy() const;

    str reversed() const;

    str& reverse();

    str operator()(size_t, size_t) const;

    str operator()(size_t) const;

    str substr(size_t, size_t) const;

    str substr(size_t) const;

    str& insert(const str&, size_t);

    str& erase(size_t, size_t);

    bool startswith(const str&) const;

    bool endswith(const str&) const;

    bool contains_char(char32_t, size_t = 0, size_t = 0) const;

    size_t count_char(char32_t, size_t = 0, size_t = 0) const;

    size_t find_char(char32_t, size_t = 0, size_t = 0) const;

    size_t rfind_char(char32_t, size_t = 0, size_t = 0) const;

    vect<size_t> find_all_char(char32_t, size_t = 0, size_t = 0) const;

    str& replace_char(char32_t, char32_t, size_t = 0, size_t = 0);

    str& rreplace_char(char32_t, char32_t, size_t = 0, size_t = 0);

    str& replace_all_chars(char32_t, char32_t, size_t = 0, size_t = 0);

    bool contains(const str&, size_t = 0, size_t = 0) const;

    size_t count(const str&, size_t = 0, size_t = 0) const;

    size_t count_intersect(const str&, size_t = 0, size_t = 0) const;

    size_t find(const str&, size_t = 0, size_t = 0) const;

    size_t rfind(const str&, size_t = 0, size_t = 0) const;

    vect<size_t> find_all(const str&, size_t = 0, size_t = 0) const;

    vect<size_t> find_all_intersect(const str&, size_t = 0, size_t = 0) const;

    str& replace(const str&, const str&, size_t = 0, size_t = 0);

    str& rreplace(const str&, const str&, size_t = 0, size_t = 0);

    str& replace_all(const str&, const str&, size_t = 0, size_t = 0);

    str& rreplace_all(const str&, const str&, size_t = 0, size_t = 0);

    vect<str> split(const str& = "\n") const;

    size_t length() const; // amount of code points

    size_t raw_length() const; // amount of code units

    uint64_t hash() const noexcept;

    void clear();

    bool empty() const;

    void swap(str&);

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    iterator rbegin();

    iterator rend();

    const_iterator rbegin() const;

    const_iterator rend() const;

    friend std::ostream& operator<<(std::ostream &out, const str&);

    friend std::istream& operator>>(std::istream &in, str&);

    void set_flags(str_flag) const;

    str_flag get_flags() const;

    static ucode_type code_point_type(char32_t) const;

    static ucode_type code_unit_type(char32_t) const;
};

std::ostream& operator<<(std::ostream &out, const str&);

std::istream& operator>>(std::istream &in, str&);
