#pragma once

#include "../template/arraymethods.hpp"
#include "../template/typemethods.hpp"
#include "../template/struct_tags.hpp"
#include "sortsquare.hpp"

template<typename T, typename = typename std::enable_if<std::is_integral<T>::value>::type>
static typename getter_func<T>::RType def_getter(T value, unsigned short part)
{
    typedef typename getter_func<T>::RType RType;

    if(std::is_signed<T>::value && (part == 0))
    {
        return static_cast<RType>(static_cast<typename std::make_unsigned<T>::type>(value) >> ((sizeof(T) - sizeof(RType)) * 8)) ^
               (static_cast<RType>(1 << (sizeof(RType) * 8 - 1)));
    }
    return static_cast<RType>(value >> (sizeof(T) + part * sizeof(RType) - sizeof(RType)) * 8);
}

template<typename T>
void countsort(T min = 0, T max = 0)
{}

void radixsort(bool right = false, bool nsigned = true);

template<typename T, typename R,
        typename getter_func<T, R>::type getter, typename compare_func<T>::type compare, typename E>
static void bucketsort_impl(T *start, T *end, unsigned short from = 0)
{
    E length = end - start;
    E *items_in_bucket = new E[std::numeric_limits<R>::max()];
    _fill(items_in_bucket, std::numeric_limits<R>::max());
    const E index_to = (sizeof(E) == sizeof(R)) ? 0 : static_cast<E>(std::numeric_limits<R>::max()) + 1;

    for(E i = 0;i < length;i++)
        items_in_bucket[getter(start[i], from)]++;
    for(E i = 1;i != index_to;i++)
        items_in_bucket[i] += items_in_bucket[i - 1];
    T *copy = new T[length];
    _move(copy, length, start);
    for(E i = 0;i < length;i++)
        start[--items_in_bucket[getter(copy[i], from)]] = std::move(copy[i]);
    if(from < getter_parts<T, R>() - 1)
    {
        if(compare)
            for(E i = 1; i != index_to; i++)
                if(items_in_bucket[i] - items_in_bucket[i - 1] < 256)
                    minmaxsort<T, compare>(start + items_in_bucket[i - 1], start + items_in_bucket[i]);
                else
                    bucketsort_impl<T, R, getter, compare, E>(start + items_in_bucket[i - 1], start + items_in_bucket[i], from + 1);
        else
            for(E i = 1; i != index_to; i++)
                if(items_in_bucket[i] - items_in_bucket[i - 1] > 1)
                    bucketsort_impl<T, R, getter, nullptr, E>(start + items_in_bucket[i - 1], start + items_in_bucket[i], from + 1);
    }
}

template<typename T, typename R = typename getter_func<T>::RType,
        typename getter_func<T, R>::type getter = def_getter<T>,
                typename compare_func<T>::type compare = def_comparator<T>::comparator>
void bucketsort(T *start, T *end)
{
    if(end - start < ((1 << 16) - 1))
        bucketsort_impl<T, R, getter, compare, u_int16_t>(start, end);
    else
        bucketsort_impl<T, R, getter, compare, u_int32_t>(start, end);
}

//void bucketsort_u(unsigned *start, unsigned *end, unsigned n = 18)
//{
//    unsigned dif = (unsigned) (end - start);
//    unsigned min, max;
//    min = max = *start;
//    for(unsigned *s = start + 1; s < end; s++)
//        if(*s < min)
//            min = *s;
//        else if(*s > max)
//            max = *s;
//    unsigned k = (max - min) / (1 << n) + 1;
//    k = 1 << (32 - n);
//    unsigned *count = new unsigned[k + 1];
//    unsigned *m = new unsigned[dif];
//    _fill(count, k);
//    for(unsigned *s = start; s < end; s++)
//    {
//        count[*s >> n]++;
//        m[s - start] = *s;
//    }
//    count[k] = dif;
//    for(unsigned i = 1; i < k; i++)
//        count[i] += count[i - 1];
//    for(unsigned *s = m; s < m + dif; s++)
//        start[--count[*s >> n]] = *s;
//    delete[] m;
//    for(unsigned i = 0; i < k; i++)
//        if(count[i] < count[i + 1] - 1)
//            insertionsort(start + count[i], start + count[i + 1]);
//    delete[] count;
//}
//
//void bucketsort_s(int *start, int *end, unsigned n = 18)
//{
//    unsigned *start_u = (unsigned *) start;
//    unsigned *end_u = (unsigned *) end;
//    unsigned dif = (unsigned) (end - start);
//    int min, max;
//    min = max = *start;
//    for(int *s = start + 1; s < end; s++)
//        if(*s < min)
//            min = *s;
//        else if(*s > max)
//            max = *s;
//    unsigned k = (max - min) / (1 << n) + 1;
//    k = 1 << (32 - n);
//    unsigned *count = new unsigned[k + 1];
//    unsigned *m = new unsigned[dif];
//    _fill(count, k);
//    for(unsigned *s = start_u; s < end_u; s++)
//    {
//        count[(*s ^ (1 << 31)) >> n]++;
//        m[s - start_u] = *s;
//    }
//    count[k] = dif;
//    for(unsigned i = 1; i < k; i++)
//        count[i] += count[i - 1];
//    for(unsigned *s = m; s < m + dif; s++)
//        start_u[--count[(*s ^ (1 << 31)) >> n]] = *s;
//    delete[] m;
//    for(unsigned i = 0; i < k; i++)
//        if(count[i] < count[i + 1] - 1)
//            insertionsort(start + count[i], start + count[i + 1]);
//    delete[] count;
//}
