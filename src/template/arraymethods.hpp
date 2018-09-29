#pragma once

#include "commonmethods.hpp"
#include "typemethods.hpp"
#include "valuemethods.hpp"
#include "../other/defdef.h"
#include "../debug/def_debug.h"
#include "struct_tags.hpp"

#include <cstring>

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

template<typename T, typename SizeType=unsigned>
inline void _vfill(T *start, SizeType len, const T x)
{
    while(len-- > 0)
        *start++ = x;
}

template<typename T, typename SizeType=unsigned,
        typename = typename std::enable_if<std::is_integral<T>::value ||
        std::is_pointer<T>::value>::type>
inline void _fill(T *start, SizeType len, const T x = 0)
{
    if(_valueMethods<T>::bytesEqual(x))
        memset(start, _valueMethods<T>::getFirstByte(x), len * sizeof(*start));
    else
        _vfill(start, len, x);
}

inline char *_readstr(unsigned &len)
{
    int c;
    char *str = new char[len];
    unsigned i = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        str[i++] = (char) c;
    len = i;
    return str;
}

template<typename T = unsigned>
inline void _read(T *start, T *end, const char *scf = _typeSeq<T>::specifier)
{
    for(T *i = start; i < end; i++)
        scanf(scf, i);
}

template<typename T = unsigned, typename SizeType=unsigned>
inline void _read(T *start, SizeType len, const char *scf = _typeSeq<T>::specifier)
{
    for(unsigned i = 0; i < len; i++)
        scanf(scf, start++);
}

template<typename T = unsigned, typename SizeType=unsigned>
inline T *_read(SizeType len, const char *scf = _typeSeq<T>::specifier)
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

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<is_bit_copyable<T>::value>::type
    _copy(T *__restrict__ start, SizeType len, const T *__restrict__ source)
{
    memcpy(start, source, len * sizeof(T));
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<!is_bit_copyable<T>::value>::type
    _copy(T *__restrict__ start, SizeType len, const T *__restrict__ source)
{
    while(len--)
        *start++ = *source++;
}

template<typename T>
inline T *_new_copy(T *start, T *end)
{
    T *new_array = new T[end - start];
    _copy(new_array, end - start, start);
    return new_array;
}

template<typename T, typename SizeType=unsigned>
inline T *_new_copy(T *start, SizeType len)
{
    T *new_array = new T[len];
    _copy(new_array, len, start);
    return new_array;
}

template<typename T, typename SizeType=unsigned>
inline T *_new_copy(T *start, T *end, SizeType new_length)
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

template<typename T, typename SizeType=unsigned, typename SizeType2=unsigned>
inline T *_new_copy(T *start, SizeType len, SizeType2 new_length)
{
    DEBUGLVLIFMSG(3, new_length < len, "new size lesser than old, some elements won't be copied!");
    T *new_array = new T[new_length];
    if(len <= new_array)
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

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<is_bit_copyable<T>::value>::type
    _copy_a(T *start, SizeType len, const T *source)
{
    memmove(start, source, len * sizeof(T));
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<!is_bit_copyable<T>::value>::type
    _copy_a(T *start, SizeType len, const T *source)
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

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<is_bit_movable<T>::value>::type
    _move(T *__restrict__ start, SizeType len, T *__restrict__ source)
{
    memcpy(start, source, len * sizeof(T));
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<!is_bit_movable<T>::value>::type
    _move(T *__restrict__ start, SizeType len, T *__restrict__ source)
{
    while(len--)
        *start++ = std::move(*source++);
}

template<typename T, typename SizeType=unsigned>
inline T* _resize(T *start, T *end, SizeType new_length)
{
    DEBUGLVLIFMSG(3, new_length < (end - start), "new size lesser than old, some elements will be deleted!");
    T *new_array = new T[new_length];
    _move(new_array, _min(end - start, new_length), start);
    delete [] start;
    return new_array;
}

template<typename T, typename SizeType=unsigned, typename SizeType2=unsigned>
inline T* _resize(T *start, SizeType now_length, SizeType2 new_length)
{
    DEBUGLVLIFMSG(3, new_length < now_length, "new size lesser than old, some elements will be deleted!");
    T *new_array = new T[new_length];
    _move(new_array, _min(now_length, new_length), start);
    delete [] start;
    return new_array;
}

template<typename T, typename SizeType=unsigned>
inline typename std::enable_if<is_bit_movable<T>::value, T*>::type
_resize_alloc(T *start, SizeType new_length)
{
    return static_cast<T*>(realloc(start, new_length));
}

template<typename T, typename SizeType=unsigned>
inline void _mult_array(T *const start, SizeType len, unsigned times)
{
    unsigned done = 1;
    unsigned times_shift = times >> 1;
    SizeType clen = len;
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
inline T& _min(T *start, unsigned long len)
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
inline T& _max(T *start, unsigned long len)
{
    T *emax = start;
    while(len-- > 1)
        if(_more(*++start, *emax))
            emax = start;
    return *emax;
}

template<typename T>
inline unsigned long _minInd(T *start, T *end)
{
    T vmin = *start;
    unsigned ind = 0;
    unsigned len = end - start;
    while(len > 0)
        if(_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
inline unsigned long _minInd(T *start, unsigned long len)
{
    T vmin = *start;
    unsigned ind = 0;
    while(len > 0)
        if(_more(vmin, start[--len]))
            vmin = start[ind = len];
    return ind;
}

template<typename T>
inline unsigned long _maxInd(T *start, T *end)
{
    T vmax = *start;
    unsigned ind = 0;
    unsigned len = end - start;
    while(len > 0)
        if(_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename T>
inline unsigned long _maxInd(T *start, unsigned long len)
{
    T vmax = *start;
    unsigned ind = 0;
    while(len > 0)
        if(_more(start[--len], vmax))
            vmax = start[ind = len];
    return ind;
}

template<typename Ta, typename Ts>
inline Ts& _sum(Ta *start, unsigned long len)
{
    Ts sum = 0;
    while(len-- > 0)
        sum += start[len];
    return sum;
}

template<typename Ta, typename Ts>
inline Ts& _sum(Ta *start, Ta *end)
{
    Ts sum = 0;
    unsigned len = end - start;
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
inline bool _checksorted(T *start, unsigned long len)
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
inline T **_newArray2d(unsigned long height, unsigned long lenght)
{
    T **array = new T *[height];
    while(height > 0)
        array[--height] = new T[lenght];
    return array;
}

template<typename T>
inline void _deleteArray2d(T **array, unsigned long height)
{
    while(height > 0)
        delete[] array[--height];
    delete[] array;
}

#ifdef INDEXARRAY

unsigned *indexArray;

#endif // INDEXARRAY

template<typename T1, typename T2>
inline void _adswap(T1 *array1, T2 *array2, unsigned f, unsigned s)
{
    T1 tmp1 = array1[f];
    array1[f] = array1[s];
    array1[s] = tmp1;
    T2 tmp2 = array2[f];
    array2[f] = array2[s];
    array2[s] = tmp2;
#ifdef COUNTSWAPS
    swapscounter+=2;
#endif // COUNTSWAPS
#ifdef INDEXARRAY
    unsigned tmpInd = indexArray[f];
    indexArray[f] = indexArray[s];
    indexArray[s] = tmpInd;
#endif // INDEXARRAY
}

template<typename T>
inline void _aswap(T *array, unsigned f, unsigned s)
{
    T tmp = array[f];
    array[f] = array[s];
    array[s] = tmp;
#ifdef COUNTSWAPS
    swapscounter++;
#endif // COUNTSWAPS
#ifdef INDEXARRAY
    unsigned tmpInd = indexArray[f];
    indexArray[f] = indexArray[s];
    indexArray[s] = tmpInd;
#endif // INDEXARRAY
}

inline bool c_str_equals(const char *const a, const char *const b)
{
    for(unsigned i = 0;; i++)
        if(a[i] != b[i])
            return false;
        elif(a[i] == '\0')
            return true;
}

template<typename T>
inline void _reverse(T *start, T *end)
{
    while(start < --end)
        std::swap(*start++, *end);
}

template<typename T, typename SizeType=unsigned>
inline void _reverse(T *start, SizeType length)
{
    SizeType from = 0;
    while(from < --length)
        std::swap(start[from++], start[length]);
}

template<typename T, typename SizeType=unsigned, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
        && std::is_integral<SizeType>::value && std::is_integral<ShiftType>::value>>
inline void shr_range(T *start, SizeType length, ShiftType shift);

template<typename T, typename SizeType=unsigned, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
        && std::is_integral<SizeType>::value && std::is_integral<ShiftType>::value>>
inline void shl_range(T *start, SizeType length, ShiftType shift)
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
        for(SizeType i = 0;i < length - shift;i++)
            start[i] = start[i + shift];
        length -= shift;
        _fill(start + length, shift);
    }
    if(shift_in_cell)
    {
        unsigned char shift_right = _typeSeq<T>::bit_length - shift_in_cell;
        for(SizeType i = 0;i < length - 1;i++)
            start[i] = (start[i] << shift_in_cell) | (start[i + 1] >> shift_right);
        start[length - 1] <<= shift_in_cell;
    }
}

template<typename T, typename SizeType=unsigned, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
        && std::is_integral<SizeType>::value && std::is_integral<ShiftType>::value>>
inline void shr_range(T *start, SizeType length, ShiftType shift)
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
        for(SizeType i = length - 1;i >= shift;i++)
            start[i] = start[i - shift];
        length -= shift;
        _fill(start, shift);
        start += shift;
    }
    if(shift_in_cell)
    {
        unsigned char shift_left = _typeSeq<T>::bit_length - shift_in_cell;
        for(SizeType i = length;i > 0;i--)
            start[i] = (start[i - 1] << shift_left) | (start[i] >> shift_in_cell);
        start[0] >>= shift_in_cell;
    }
}

template<typename T, typename ShiftType=unsigned>
inline void shl_range(T *start,T *end, ShiftType shift)
{
    shl_range(start, end - start, shift);
}
template<typename T, typename ShiftType=unsigned>
inline void shr_range(T *start,T *end, ShiftType shift)
{
    shr_range(start, end - start, shift);
}

template<typename T, typename SizeType=unsigned, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
        && std::is_integral<SizeType>::value && std::is_integral<ShiftType>::value>>
void ror_range(T *start, SizeType length, ShiftType shift);

template<typename T, typename SizeType=unsigned, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
        && std::is_integral<SizeType>::value && std::is_integral<ShiftType>::value>>
void rol_range(T *start, SizeType length, ShiftType shift)
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
        for(SizeType i = 0;i < length - 1;i++)
            start[i] = (start[i] << shift_in_cell) | (start[i + 1] >> shift_right);
        start[length - 1] = (start[length - 1] << shift_in_cell) | (saved >> shift_right);
    }
    shift >>= _typeSeq<T>::pwr2_length;
    if(shift >= length)
        shift %= length;
    if(shift)
    {
        SizeType left = length, initial = 0;
        do
        {
            T saved = start[initial];
            SizeType i = initial, next = initial + shift;
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

template<typename T, typename SizeType=unsigned, typename ShiftType=unsigned,
    typename = typename std::enable_if_t<std::is_unsigned<T>::value
        && std::is_integral<SizeType>::value && std::is_integral<ShiftType>::value>>
void ror_range(T *start, SizeType length, ShiftType shift)
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
        for(SizeType i = length;i > 0;i--)
            start[i] = (start[i - 1] << shift_left) | (start[i] >> shift_in_cell);
        start[0] = (saved << shift_left) | (start[0] >> shift_in_cell);
    }
    shift >>= _typeSeq<T>::pwr2_length;
    if(shift >= length)
        shift %= length;
    if(shift)
    {
        SizeType left = length, initial = length - 1, len_sub_shift = length - shift;
        do
        {
            T saved = start[initial];
            SizeType i = initial, next = initial;
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
void merge_seq_two(const T* __restrict__ start, unsigned part_len1,
                   unsigned part_len2, T* __restrict__ destination)
{
    unsigned i = 0, i1 = 0, i2 = part_len1;
    while((part_len1) && (part_len2))
    {
        if(compare(start[i1], start[i2]))
        {
            destination[i++] = start[i1++];
            part_len1--;
        }
        else
        {
            destination[i++] = start[i2++];
            part_len2--;
        }
    }
    while(part_len1--)
        destination[i++] = start[i1++];
    while(part_len2--)
        destination[i++] = start[i2++];
};


//merge(first[0..part_len1], second[0..part_len2]) -> destination
//sort_in_place = true => destination[part_len2..] = second[0..]
template<typename T, bool (*compare)(T, T) = _less<T>,
                bool sort_in_place = false>
void merge_two_arrays(T *first, unsigned part_len1, T *second, unsigned part_len2, T *destination)
{
    while((part_len1) && (part_len2))
    {
        if(compare(*first, *second))
        {
            *(destination++) = *(first++);
            part_len1--;
        }
        else
        {
            *(destination++) = *(second++);
            part_len2--;
        }
    }
    while(part_len1--)
        *(destination++) = *(first++);
    if(!sort_in_place)
        while(part_len2--)
            *(destination++) = *(second++);
};

template<typename T, bool (*compare)(T, T) = _less<T>, typename SizeType=unsigned>
unsigned long long merge_with_inversions_count(const T* __restrict__ first, SizeType len1,
                            const T* __restrict__ second, SizeType len2, const T* __restrict__ destination)
{
    unsigned long long swaps = 0;
    while((len1) && (len2))
        if(compare(*second, *first))
        {
            *(destination++) = *(second++);
            swaps += len1;
            len2--;
        }
        else
        {
            *(destination++) = *(first++);
            len1--;
        }
        while(len1--)
            *(destination++) = *(first++);
        while(len2--)
            *(destination++) = *(second++);
        return swaps;
}

template<typename T>
unsigned long long inversions_count(T *start, T *end)
{
    unsigned long long inversions = 0;
    unsigned long block_size = 1, dif = end - start;
    T *swp = new T[dif / 2];
    while(block_size < dif)
    {
        _copy(start, dif / 2, swp);

    }
    return 0;
}
