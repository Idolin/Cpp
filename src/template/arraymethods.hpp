#pragma once

#include "commonmethods.hpp"
#include "typemethods.hpp"
#include "valuemethods.hpp"
#include "../other/defdef.h"
#include "../debug/def_debug.h"
#include "struct_tags.hpp"

#include <cstring>
#include <type_traits>

template<typename T>
inline void _vfill(T *start, T *end, const T x)
{
    while(start < end)
        *start++ = x;
}

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value ||
        std::is_pointer<T>::value>::type>
inline void _fill(T *start, T *end, const T x = 0)
{
    if(_valueMethods<T>::bytesEqual(x))
        memset(start, _valueMethods<T>::getFirstByte(x), (end - start) * sizeof(*start));
    else
        _vfill(start, end, x);
}

template<typename T>
inline void _vfill(T *start, size_t len, const T x)
{
    while(len-- > 0)
        *start++ = x;
}

template<typename T>
inline void _fill(T *start, size_t len, const T x = 0)
{
    if(_valueMethods<T>::bytesEqual(x))
        memset(start, _valueMethods<T>::getFirstByte(x), len * sizeof(*start));
    else
        _vfill(start, len, x);
}

inline char* _readstr(size_t &len)
{
    int c;
    char *str = new char[len];
    len = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        str[len++] = (char) c;
    return str;
}

template<typename T>
inline void _read(T *start, T *end, const char *scf = _typeSeq<T>::specifier)
{
    for(T *i = start; i < end; i++)
        scanf(scf, i);
}

template<typename T>
inline void _read(T *start, size_t len, const char *scf = _typeSeq<T>::specifier)
{
    for(size_t i = 0; i < len; i++)
        scanf(scf, start++);
}

template<typename T>
inline T* _read(size_t len, const char *scf = _typeSeq<T>::specifier)
{
    T *r = new T[len];
    for(unsigned i = 0; i < len; i++)
        scanf(scf, r + i);
    return r;
}

template<typename T>
inline typename std::enable_if<is_bit_copyable<T>::value>::type
    _copy(T *__restrict__ start, T *end, const T *__restrict__ source)
{
    memcpy(start, source, (end - start) * sizeof(T));
}

template<typename T>
inline typename std::enable_if<!is_bit_copyable<T>::value>::type
    _copy(T *__restrict__ start, T *end, const T *__restrict__ source)
{
    while(start < end)
        *start++ = *source++;
}

template<typename T>
inline typename std::enable_if<is_bit_copyable<T>::value>::type
    _copy(T *__restrict__ start, size_t len, const T *__restrict__ source)
{
    ASSERT(static_cast<unsigned long long>(len * sizeof(T)) > 9223372036854775807,
           "Specified size %llu exceeds maximum object size 9223372036854775807",
                static_cast<unsigned long long>(len * sizeof(T)));
    memcpy(start, source, len * sizeof(T));
}

template<typename T>
inline typename std::enable_if<!is_bit_copyable<T>::value>::type
    _copy(T *__restrict__ start, size_t len, const T *__restrict__ source)
{
    while(len--)
        *start++ = *source++;
}

template<typename T>
inline T *_new_copy(const T *start, const T *end)
{
    T *new_array = new T[end - start];
    _copy(new_array, end - start, start);
    return new_array;
}

template<typename T>
inline T *_new_copy(const T *start, size_t len)
{
    T *new_array = new T[len];
    _copy(new_array, len, start);
    return new_array;
}

template<typename T>
inline T* _new_copy(const T *start, const T *end, size_t new_length)
{
    DEBUGLVLIFMSG(3, new_length < (end - start), "new size lesser than old, some elements won't be copied!");
    T *new_array = new T[new_length];
    unsigned long len = end - start;
    if(new_length <= len)
        _copy(new_array, new_length, start);
    else
    {
        _copy(new_array, len, start);
        _fill(new_array + len, new_length - len);
    }
    return new_array;
}

template<typename T>
inline T* _new_copy(const T *start, size_t len, size_t new_length)
{
    DEBUGLVLIFMSG(3, new_length < len, "new size lesser than old, some elements won't be copied!");
    T *new_array = new T[new_length];
    if(new_length <= len)
        _copy(new_array, _min(len, new_length), start);
    else
    {
        _copy(new_array, len, start);
        _fill(new_array + len, new_length - len);
    }
    return new_array;
}

template<typename T>
inline typename std::enable_if<is_bit_copyable<T>::value>::type
    _copy_a(T *start, T *end, const T *source)
{
    memmove(start, source, (end - start) * sizeof(T));
}

template<typename T>
inline typename std::enable_if<!is_bit_copyable<T>::value>::type
    _copy_a(T *start, T *end, const T *source)
{
    if(start < source)
        while(start < end)
            *start++ = *source++;
    else
    {
        source += end - start;
        while(start < end)
            *--end = *--source;
    }
}

template<typename T>
inline typename std::enable_if<is_bit_copyable<T>::value>::type
    _copy_a(T *start, size_t len, const T *source)
{
    memmove(start, source, len * sizeof(T));
}

template<typename T>
inline typename std::enable_if<!is_bit_copyable<T>::value>::type
    _copy_a(T *start, size_t len, const T *source)
{
    if(start < source)
        while(len--)
            *start++ = *source++;
    else
    {
        source += len;
        start += len;
        while(len--)
            *--start = *--source;
    }
}

template<typename T>
inline typename std::enable_if<is_bit_movable<T>::value>::type
    _move(T *__restrict__ start, T *end, T *__restrict__ source)
{
    memcpy(start, source, (end - start) * sizeof(T));
}

template<typename T>
inline typename std::enable_if<!is_bit_movable<T>::value>::type
    _move(T *__restrict__ start, T *end, T *__restrict__ source)
{
    while(start < end)
        *start++ = std::move(*source++);
}

template<typename T>
inline typename std::enable_if<is_bit_movable<T>::value>::type
    _move(T *__restrict__ start, size_t len, T *__restrict__ source)
{
    memcpy(start, source, len * sizeof(T));
}

template<typename T>
inline typename std::enable_if<!is_bit_movable<T>::value>::type
    _move(T *__restrict__ start, size_t len, T *__restrict__ source)
{
    while(len--)
        *start++ = std::move(*source++);
}

template<typename T>
inline T* _resize(T *start, T *end, size_t new_length)
{
    DEBUGLVLIFMSG(3, new_length < (end - start), "new size lesser than old, some elements will be deleted!");
    T *new_array = new T[new_length];
    _move(new_array, _min(end - start, new_length), start);
    delete [] start;
    return new_array;
}

template<typename T>
inline T* _resize(T *start, size_t now_length, size_t new_length)
{
    DEBUGLVLIFMSG(3, new_length < now_length, "new size lesser than old, some elements will be deleted!");
    T *new_array = new T[new_length];
    _move(new_array, _min(now_length, new_length), start);
    delete [] start;
    return new_array;
}

template<typename T>
inline typename std::enable_if<is_bit_movable<T>::value, T*>::type
_resize_alloc(T *start, size_t new_length)
{
    return static_cast<T*>(realloc(start, new_length));
}

template<typename T>
inline void _mult_array(T *const start, size_t len, unsigned times)
{
    unsigned done = 1;
    unsigned times_shift = times >> 1;
    size_t clen = len;
    while(done <= times_shift)
    {
        _copy_a(start + clen, clen, start);
        clen <<= 1;
        done <<= 1;
    }
    if(times -= done)
        _copy_a(start + clen, len * times, start);
}

template<typename T>
inline T& _min(T *start, T *end)
{
    T *emin = start;
    while(++start < end)
        if(_more(*emin, *start))
            emin = start;
    return *emin;
}

template<typename T>
inline T& _min(T *start, size_t len)
{
    T *emin = start;
    while(len-- > 1)
        if(_more(*emin, *++start))
            emin = start;
    return *emin;
}

template<typename T>
inline T& _max(T *start, T *end)
{
    T *emax = start;
    while(++start < end)
        if(_more(*start, *emax))
            emax = start;
    return *emax;
}

template<typename T>
inline T& _max(T *start, size_t len)
{
    T *emax = start;
    while(len-- > 1)
        if(_more(*++start, *emax))
            emax = start;
    return *emax;
}

template<typename T>
inline size_t _minInd(T *start, T *end)
{
    T vmin = *start;
    size_t ind = 0, len = end - start;
    while(len > 0)
        if(_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
inline size_t _minInd(T *start, size_t len)
{
    T vmin = *start;
    size_t ind = 0;
    while(len > 0)
        if(_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
inline size_t _maxInd(T *start, T *end)
{
    T vmax = *start;
    size_t ind = 0, len = end - start;
    while(len > 0)
        if(_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename T>
inline size_t _maxInd(T *start, size_t len)
{
    T vmax = *start;
    size_t ind = 0;
    while(len > 0)
        if(_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename R, typename T>
inline R _sum(const T *start, size_t len)
{
    R sum = 0;
    while(len-- > 0)
        sum += start[len];
    return sum;
}

template<typename R, typename T>
inline R _sum(const T *start, const T *end)
{
    R sum = 0;
    size_t len = end - start;
    while(len-- > 0)
        sum += start[len];
    return sum;
}

template<typename T, typename compare_func<T>::type compare = _less<T>>
inline bool _checksorted(T *start, T *end)
{
    while(++start < end)
        if(compare(*start, *(start - 1)))
            return false;
    return true;
}

template<typename T, typename compare_func<T>::type compare = _less<T>>
inline bool _checksorted(T *start, size_t len)
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
inline T **_newArray2d(size_t height, size_t lenght)
{
    T **array = new T *[height];
    while(height > 0)
        array[--height] = new T[lenght];
    return array;
}

template<typename T>
inline void _deleteArray2d(T **array, size_t height)
{
    while(height > 0)
        delete[] array[--height];
    delete[] array;
}

inline bool c_str_equals(const char *a, const char *b)
{
    for(size_t i = 0;; i++)
        if(a[i] != b[i])
            return false;
        elif(a[i] == '\0')
            return true;
}

inline bool c_str_equals(const char *a, const char *b, size_t compare_to)
{
    for(size_t i = 0;i < compare_to;i++)
        if(a[i] != b[i])
            return false;
        elif(a[i] == '\0')
            return true;
    return true;
}

template<typename T>
inline void _reverse(T *start, T *end)
{
    while(start < --end)
        std::swap(*start++, *end);
}

template<typename T>
inline void _reverse(T *start, size_t length)
{
    size_t from = 0;
    while(from < --length)
        std::swap(start[from++], start[length]);
}

template<typename T, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value>>
inline void shr_range(T *start, size_t length, ShiftType shift);

template<typename T, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value>>
inline void shl_range(T *start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        shr_range(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    unsigned char shift_in_cell = shift & (_typeSeq<T>::bit_length - 1);
    shift >>= _typeSeq<T>::pwr2_length;
    if(shift >= length)
    {
        _fill(start, length);
        return;
    }
    if(shift)
    {
        for(size_t i = 0;i < length - shift;i++)
            start[i] = start[i + shift];
        length -= shift;
        _fill(start + length, shift);
    }
    if(shift_in_cell)
    {
        unsigned char shift_right = _typeSeq<T>::bit_length - shift_in_cell;
        for(size_t i = 0;i < length - 1;i++)
            start[i] = (start[i] << shift_in_cell) | (start[i + 1] >> shift_right);
        start[length - 1] <<= shift_in_cell;
    }
}

template<typename T, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value>>
inline void shr_range(T *start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        shl_range(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    unsigned char shift_in_cell = shift & (_typeSeq<T>::bit_length - 1);
    shift >>= _typeSeq<T>::pwr2_length;
    if(shift >= length)
    {
        _fill(start, length);
        return;
    }
    if(shift)
    {
        for(size_t i = length - 1;i >= shift;i++)
            start[i] = start[i - shift];
        length -= shift;
        _fill(start, shift);
        start += shift;
    }
    if(shift_in_cell)
    {
        unsigned char shift_left = _typeSeq<T>::bit_length - shift_in_cell;
        for(size_t i = length;i > 0;i--)
            start[i] = (start[i - 1] << shift_left) | (start[i] >> shift_in_cell);
        start[0] >>= shift_in_cell;
    }
}

template<typename T, typename ShiftType=unsigned>
inline void shl_range(T *start, T *end, ShiftType shift)
{
    shl_range(start, end - start, shift);
}
template<typename T, typename ShiftType=unsigned>
inline void shr_range(T *start, T *end, ShiftType shift)
{
    shr_range(start, end - start, shift);
}

template<typename T, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value>>
void ror_range(T *start, size_t length, ShiftType shift);

template<typename T, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value>>
void rol_range(T *start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        ror_range(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    if(unsigned char shift_in_cell = shift & (_typeSeq<T>::bit_length - 1))
    {
        T saved = start[0];
        unsigned char shift_right = _typeSeq<T>::bit_length - shift_in_cell;
        for(size_t i = 0;i < length - 1;i++)
            start[i] = (start[i] << shift_in_cell) | (start[i + 1] >> shift_right);
        start[length - 1] = (start[length - 1] << shift_in_cell) | (saved >> shift_right);
    }
    shift >>= _typeSeq<T>::pwr2_length;
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
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
         && std::is_integral<ShiftType>::value>>
void ror_range(T *start, size_t length, ShiftType shift)
{
    if(length == 0)
        return;
    if(shift < 0)
    {
        rol_range(start, length, -shift);
        return;
    }
    ASSERT(length > 0);
    if(unsigned char shift_in_cell = shift & (_typeSeq<T>::bit_length - 1))
    {
        T saved = start[length - 1];
        unsigned char shift_left = _typeSeq<T>::bit_length - shift_in_cell;
        for(size_t i = length;i > 0;i--)
            start[i] = (start[i - 1] << shift_left) | (start[i] >> shift_in_cell);
        start[0] = (saved << shift_left) | (start[0] >> shift_in_cell);
    }
    shift >>= _typeSeq<T>::pwr2_length;
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
inline void rol_range(T *start, T *end, ShiftType shift)
{
    rol_range(start, end - start, shift);
}

template<typename T, typename ShiftType=unsigned>
inline void ror_range(T *start, T *end, ShiftType shift)
{
    ror_range(start, end - start, shift);
}

//merge(start[0..part_len1], start[part_len1..part_len1 + part_len2]) -> destination
template<typename T, bool (*compare)(T, T) = _less<T>>
void merge_seq_two(const T* __restrict__ start, size_t part_len1,
                   size_t part_len2, T* __restrict__ destination)
{
    size_t i_dest = 0, i1 = 0, i2 = part_len1;
    while((part_len1) && (part_len2))
    {
        if(compare(start[i1], start[i2]))
        {
            destination[i_dest++] = start[i1++];
            part_len1--;
        }
        else
        {
            destination[i_dest++] = start[i2++];
            part_len2--;
        }
    }
    while(part_len1--)
        destination[i_dest++] = start[i1++];
    while(part_len2--)
        destination[i_dest++] = start[i2++];
};


//merge(first[0..part_len1], second[0..part_len2]) -> destination
//sort_in_place = true => destination[part_len2..] = second[0..]
template<typename T, typename compare_func<T>::type compare = _less<T>,
                bool sort_in_place = false, bool count_inversions = false>
typename std::conditional<count_inversions, unsigned long long, void>::type
    merge_two_arrays(T *first, size_t part_len1, T *second, size_t part_len2, T *destination)
{
    unsigned long long inversions = 0;
    while((part_len1) && (part_len2))
    {
        if(compare(*second, *first))
        {
            *(destination++) = *(second++);
            if(count_inversions)
                inversions += part_len1;
            part_len2--;
        }
        else
        {
            *(destination++) = *(first++);
            part_len1--;
        }
    }
    while(part_len1--)
        *(destination++) = *(first++);
    if(!sort_in_place)
        while(part_len2--)
            *(destination++) = *(second++);
    if(count_inversions)
        return static_cast<typename std::conditional<count_inversions, unsigned long long, void>::type>(inversions);
    else
        return static_cast<typename std::conditional<count_inversions, unsigned long long, void>::type>(0);
};