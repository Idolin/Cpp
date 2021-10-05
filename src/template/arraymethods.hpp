#pragma once

#include "commonmethods.hpp"
#include "typemethods.hpp"
#include "valuemethods.hpp"
#include "../other/common.hpp"
#include "../debug/def_debug.h"
#include "type_tags.hpp"

#include <cstring>
#include <type_traits>

#ifdef _MSC_VER
    #define __restrict__ __restrict
#endif

using std::size_t;

template<typename T>
inline void fill_with_value(T* start, T* end, const T& x)
{
    while(start < end)
        *start++ = x;
}

template<typename T,
         typename = std::enable_if_t<std::is_integral<T>::value || std::is_pointer<T>::value>>
inline void fill(T* start, T* end, T x = 0)
{
    if(_valueMethods<T>::bytesEqual(x))
        memset(start, _valueMethods<T>::getFirstByte(x), (end - start) * sizeof(*start));
    else
        fill_with_value(start, end, x);
}

template<typename T>
inline void fill_with_value(T* start, size_t len, const T& x)
{
    while(len-- > 0)
        *start++ = x;
}

template<typename T>
inline void fill(T* start, size_t len, const T& x = 0)
{
    if(_valueMethods<T>::bytesEqual(x))
        memset(start, _valueMethods<T>::getFirstByte(x), len * sizeof(*start));
    else
        fill_with_value(start, len, x);
}

inline char* readcstr(size_t& len)
{
    ASSERT(len > 0);
    int c;
    char *str = new char[len];
    len = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        str[len++] = (char) c;
    return str;
}

template<typename T>
inline void read_to_array(T* start, T* end, const char* scf = type_info<T>::specifier)
{
    for(T *i = start; i < end; i++)
        scanf(scf, i);
}

template<typename T>
inline void read_to_array(T* start, size_t len, const char* scf = type_info<T>::specifier)
{
    for(size_t i = 0; i < len; i++)
        scanf(scf, start++);
}

template<typename T>
inline T* read_to_array(size_t len, const char* scf = type_info<T>::specifier)
{
    T *r = new T[len];
    for(size_t i = 0; i < len; i++)
        scanf(scf, r + i);
    return r;
}

template<typename T>
inline std::enable_if_t<is_bit_copyable<T>::value>
    copy_array(const T *__restrict__ source, T* end, T *__restrict__ destination)
{
    memcpy(destination, source, (end - source) * sizeof(T));
}

template<typename T>
inline std::enable_if_t<!is_bit_copyable<T>::value>
    copy_array(const T *__restrict__ source, T* end, T *__restrict__ destination)
{
    while(source < end)
        *destination++ = *source++;
}

template<typename T>
inline typename std::enable_if<is_bit_copyable<T>::value>::type
    copy_array(const T *__restrict__ source, size_t len, T *__restrict__ destination)
{
    ASSERT(static_cast<unsigned long long>(len * sizeof(T)) < 9223372036854775807,
           "Specified size %llu exceeds maximum object size 9223372036854775807",
                static_cast<unsigned long long>(len * sizeof(T)));
    memcpy(destination, source, len * sizeof(T));
}

template<typename T>
inline typename std::enable_if<!is_bit_copyable<T>::value>::type
    copy_array(const T *__restrict__ source, size_t len, T *__restrict__ destination)
{
    while(len--)
        *destination++ = *source++;
}

template<typename T>
inline T* new_array_copy(const T* source, const T* end)
{
    T *new_array = new T[end - source];
    copy_array(source, end - source, new_array);
    return new_array;
}

template<typename T>
inline T* new_array_copy(const T* source, size_t len)
{
    T *new_array = new T[len];
    copy_array(source, len, new_array);
    return new_array;
}

template<typename T>
inline T* new_array_copy(const T* source, size_t len, size_t new_length)
{
    DEBUGLVLIFMSG(3, new_length < len, "new size lesser than old, some elements won't be copied!");
    T *new_array = new T[new_length];
    if(new_length <= len)
        copy_array(source, min(len, new_length), new_array);
    else
    {
        copy_array(source, len, new_array);
        fill(new_array + len, new_length - len);
    }
    return new_array;
}

template<typename T>
inline T* new_array_copy(const T* source, const T* end, size_t new_length)
{
    return new_array_copy(source, end - source, new_length);
}

template<typename T>
inline std::enable_if_t<is_bit_copyable<T>::value>
    copy_array_overlapping(const T* source, const T* end, T* destination)
{
    memmove(destination, source, (end - source) * sizeof(T));
}

template<typename T>
inline std::enable_if_t<!is_bit_copyable<T>::value>
    copy_array_overlapping(const T* source, const T* end, T* destination)
{
    if(destination < source)
        while(source < end)
            *destination++ = *source++;
    else
    {
        destination += end - source;
        while(source < end)
            *destination-- = *--end;
    }
}

template<typename T>
inline std::enable_if_t<is_bit_copyable<T>::value>
    copy_array_overlapping(const T *source, size_t len, T *destination)
{
    memmove(destination, source, len * sizeof(T));
}

template<typename T>
inline std::enable_if_t<!is_bit_copyable<T>::value>
    copy_array_overlapping(const T* source, size_t len, T* destination)
{
    if(destination < source)
        while(len--)
            *destination++ = *source++;
    else
    {
        source += len;
        destination += len;
        while(len--)
            *--destination = *--source;
    }
}

template<typename T>
inline std::enable_if_t<is_bit_movable<T>::value>
    move_array(T *__restrict__ source, T* end, T *__restrict__ destination)
{
    memcpy(destination, source, (end - source) * sizeof(T));
}

template<typename T>
inline std::enable_if_t<!is_bit_movable<T>::value>
    move_array(T *__restrict__ source, T* end, T *__restrict__ destination)
{
    while(source < end)
        *destination++ = std::move(*source++);
}

template<typename T>
inline std::enable_if_t<is_bit_movable<T>::value>
    move_array(T *__restrict__ source, size_t len, T *__restrict__ destination)
{
    memcpy(destination, source, len * sizeof(T));
}

template<typename T>
inline std::enable_if_t<!is_bit_movable<T>::value>
    move_array(T *__restrict__ source, size_t len, T *__restrict__ destination)
{
    while(len--)
        *destination++ = std::move(*source++);
}

template<typename T>
inline T* resize_array(T *source, size_t now_length, size_t new_length)
{
    ASSERT(new_length > 0);
    DEBUGLVLIFMSG(3, new_length < now_length, "New size lesser than old, some elements will be deleted!");
    T *new_array = new T[new_length];
    move_array(source, min(now_length, new_length), new_array);
    delete[] source;
    return new_array;
}

template<typename T>
inline T* resize_array(T* source, T* end, size_t new_length)
{
    ASSERT(end >= source);
    return resize_array(source, end - source, new_length);
}

template<typename T>
inline std::enable_if_t<is_bit_movable<T>::value, T*>
resize_array_alloc(T* source, size_t new_length)
{
    return reinterpret_cast<T*>(realloc(source, new_length));
}

template<typename T>
inline void copy_array_n_times(T* start, size_t len, unsigned times)
{
    unsigned done = 1;
    unsigned times_shift = times >> 1;
    size_t clen = len;
    while(done <= times_shift)
    {
        copy_array(start, clen, start + clen);
        clen <<= 1;
        done <<= 1;
    }
    if(times -= done)
        copy_array(start, len * times, start + clen);
}

template<typename T>
const T& min(const T* start, const T* end)
{
    T *emin = start;
    while(++start < end)
        if(def_more(*emin, *start))
            emin = start;
    return *emin;
}

template<typename T>
const T& min(const T* start, size_t len)
{
    T *emin = start;
    while(len-- > 1)
        if(def_more(*emin, *++start))
            emin = start;
    return *emin;
}

template<typename T>
const T& max(const T* start, const T* end)
{
    T *emax = start;
    while(++start < end)
        if(def_more(*start, *emax))
            emax = start;
    return *emax;
}

template<typename T>
const T& max(const T* start, size_t len)
{
    T *emax = start;
    while(len-- > 1)
        if(def_more(*++start, *emax))
            emax = start;
    return *emax;
}

template<typename T>
size_t index_of_min(const T* start, const T* end)
{
    T vmin = *start;
    size_t ind = 0, len = end - start;
    while(len > 0)
        if(def_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
size_t index_of_min(const T* start, size_t len)
{
    T vmin = *start;
    size_t ind = 0;
    while(len > 0)
        if(def_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
size_t index_of_max(const T* start, const T* end)
{
    T vmax = *start;
    size_t ind = 0, len = end - start;
    while(len > 0)
        if(def_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename T>
size_t index_of_max(const T* start, size_t len)
{
    T vmax = *start;
    size_t ind = 0;
    while(len > 0)
        if(def_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename R, typename T>
R array_sum(const T* start, size_t len)
{
    R sum = 0;
    while(len-- > 0)
        sum += start[len];
    return sum;
}

template<typename R, typename T>
R array_sum(const T* start, const T* end)
{
    R sum = 0;
    size_t len = end - start;
    while(len-- > 0)
        sum += start[len];
    return sum;
}

template<typename T, typename compare_func<T>::type compare = def_less<T>>
bool array_checksorted(const T* start, const T* end)
{
    while(++start < end)
        if(compare(*start, *(start - 1)))
            return false;
    return true;
}

template<typename T, typename compare_func<T>::type compare = def_less<T>>
bool array_checksorted(const T* start, size_t len)
{
    while(len-- > 0)
    {
        if(compare(*(start + 1), *start))
            return false;
        start++;
    }
    return true;
}

template<typename T>
T** new_2d_array(size_t height, size_t lenght)
{
    T **array = new T *[height];
    while(height > 0)
        array[--height] = new T[lenght];
    return array;
}

template<typename T>
void delete_2d_array(T** array, size_t height)
{
    while(height > 0)
        delete[] array[--height];
    delete[] array;
}

template<typename T>
bool c_str_equals(const T* a, const T* b)
{
    for(size_t i = 0;; i++)
        if(a[i] != b[i])
            return false;
        elif(a[i] == 0)
            return true;
}

template<typename T>
bool c_str_equals(const T* a, const T* b, size_t compare_to)
{
    for(size_t i = 0;i < compare_to;i++)
        if(a[i] != b[i])
            return false;
        elif(a[i] == 0)
            return true;
    return true;
}

template<typename T>
void reverse_array(T* start, T* end)
{
    while(start < --end)
        std::swap(*start++, *end);
}

template<typename T>
void reverse_array(T* start, size_t length)
{
    size_t from = 0;
    while(from < --length)
        std::swap(start[from++], start[length]);
}

template<typename T, typename ShiftType=unsigned,
    typename = std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value>>
void array_shift_left(T* start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        array_shift_right(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    unsigned char shift_in_cell = shift & (type_info<T>::bit_length - 1);
    shift >>= type_info<T>::pwr2_length;
    if(shift >= length)
    {
        fill(start, length);
        return;
    }
    if(shift)
    {
        for(size_t i = 0;i < length - shift;i++)
            start[i] = start[i + shift];
        length -= shift;
        fill(start + length, shift);
    }
    if(shift_in_cell)
    {
        unsigned char shift_right = type_info<T>::bit_length - shift_in_cell;
        for(size_t i = 0;i < length - 1;i++)
            start[i] = (start[i] << shift_in_cell) | (start[i + 1] >> shift_right);
        start[length - 1] <<= shift_in_cell;
    }
}

template<typename T, typename ShiftType=unsigned,
    std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value, bool> = true>
void array_shift_right(T* start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        array_shift_left(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    unsigned char shift_in_cell = shift & (type_info<T>::bit_length - 1);
    shift >>= type_info<T>::pwr2_length;
    if(shift >= length)
    {
        fill(start, length);
        return;
    }
    if(shift)
    {
        for(size_t i = length - 1;i >= shift;i++)
            start[i] = start[i - shift];
        length -= shift;
        fill(start, shift);
        start += shift;
    }
    if(shift_in_cell)
    {
        unsigned char shift_left = type_info<T>::bit_length - shift_in_cell;
        for(size_t i = length;i > 0;i--)
            start[i] = (start[i - 1] << shift_left) | (start[i] >> shift_in_cell);
        start[0] >>= shift_in_cell;
    }
}

template<typename T, typename ShiftType=unsigned>
void array_shift_left(T* start, T* end, ShiftType shift)
{
    array_shift_left(start, end - start, shift);
}
template<typename T, typename ShiftType=unsigned>
void array_shift_right(T* start, T* end, ShiftType shift)
{
    array_shift_right(start, end - start, shift);
}

template<typename T, typename ShiftType=unsigned,
    std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value, bool> = true>
void array_rotate_left(T* start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        array_rotate_right(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    if(unsigned char shift_in_cell = shift & (type_info<T>::bit_length - 1))
    {
        T saved = start[0];
        unsigned char shift_right = type_info<T>::bit_length - shift_in_cell;
        for(size_t i = 0;i < length - 1;i++)
            start[i] = (start[i] << shift_in_cell) | (start[i + 1] >> shift_right);
        start[length - 1] = (start[length - 1] << shift_in_cell) | (saved >> shift_right);
    }
    shift >>= type_info<T>::pwr2_length;
    if(shift >= length)
        shift %= length;
    if(shift)
    {
        size_t left = length, initial = 0;
        do
        {
            T saved = start[initial];
            size_t i = initial, next = initial + shift;
            if(next >= length)
                next -= length;
            while(next != initial)
            {
                start[i] = start[next];
                i = next, next += shift;
                if(next >= length)
                    next -= length;
                left--;
            }
            start[i] = saved;
            initial++;
        } while(--left);
    }
}

template<typename T, typename ShiftType=unsigned,
    std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value, bool> = true>
void array_rotate_right(T *start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        array_rotate_left(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    if(unsigned char shift_in_cell = shift & (type_info<T>::bit_length - 1))
    {
        T saved = start[length - 1];
        unsigned char shift_left = type_info<T>::bit_length - shift_in_cell;
        for(size_t i = length;i > 0;i--)
            start[i] = (start[i - 1] << shift_left) | (start[i] >> shift_in_cell);
        start[0] = (saved << shift_left) | (start[0] >> shift_in_cell);
    }
    shift >>= type_info<T>::pwr2_length;
    if(shift >= length)
        shift %= length;
    if(shift)
    {
        size_t left = length, initial = length - 1, len_sub_shift = length - shift;
        do
        {
            T saved = start[initial];
            size_t i = initial, next = initial;
            if(next < shift)
                next += len_sub_shift;
            else
                next -= shift;
            while(next != initial)
            {
                start[i] = start[next];
                i = next;
                if(next < shift)
                    next += len_sub_shift;
                else
                    next -= shift;
                left--;
            }
            start[i] = saved;
            initial++;
        } while(--left);
    }
}

template<typename T, typename ShiftType=unsigned>
void array_rotate_left(T* start, T* end, ShiftType shift)
{
    array_rotate_left(start, end - start, shift);
}

template<typename T, typename ShiftType=unsigned>
void array_rotate_right(T* start, T* end, ShiftType shift)
{
    array_rotate_right(start, end - start, shift);
}

// merge(source[0..part_len1], source[part_len1..part_len1 + part_len2]) -> destination
template<typename T, bool (*compare)(T, T) = def_less<T>>
void merge_seq_two(const T* __restrict__ source, size_t part_len1,
                   size_t part_len2, T* __restrict__ destination)
{
    size_t i_dest = 0, i1 = 0, i2 = part_len1;
    while((part_len1) && (part_len2))
    {
        if(compare(source[i1], source[i2]))
        {
            destination[i_dest++] = source[i1++];
            part_len1--;
        }
        else
        {
            destination[i_dest++] = source[i2++];
            part_len2--;
        }
    }
    while(part_len1--)
        destination[i_dest++] = source[i1++];
    while(part_len2--)
        destination[i_dest++] = source[i2++];
};


// merge(source1[0..part_len1], source2[0..part_len2]) -> destination
// sort_in_place = true => destination[part_len2..] = source2[0..]
template<typename T, typename compare_func<T>::type compare = def_less<T>,
                bool sort_in_place = false, bool count_inversions = false>
typename std::conditional<count_inversions, unsigned long long, void>::type
    merge_two_arrays(T* source1, size_t part_len1, T* source2, size_t part_len2, T* destination)
{
    unsigned long long inversions = 0;
    while((part_len1) && (part_len2))
    {
        if(compare(*source2, *source1))
        {
            *(destination++) = *(source2++);
            if(count_inversions)
                inversions += part_len1;
            part_len2--;
        }
        else
        {
            *(destination++) = *(source1++);
            part_len1--;
        }
    }
    while(part_len1--)
        *(destination++) = *(source1++);
    if(!sort_in_place)
        while(part_len2--)
            *(destination++) = *(source2++);
    if(count_inversions)
        return static_cast<typename std::conditional<count_inversions, unsigned long long, void>::type>(inversions);
    else
        return static_cast<typename std::conditional<count_inversions, unsigned long long, void>::type>(0);
};
