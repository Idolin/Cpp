//
// Created by cynder on 21.06.19.
//

#include "bit_array.h"
#include "../template/typemethods.hpp"
#include "../template/arraymethods.hpp"
#include "../debug/def_debug.h"

static size_t get_bit_array_size(size_t size)
{
    return (size + type_info<unsigned>::bit_length - 1) >> type_info<unsigned>::pwr2_length;
}

bit_array::bit_array(size_t size):
        b_array(new unsigned[get_bit_array_size(size)]),
        b_size(size)
{
    _fill(b_array, get_bit_array_size(b_size));
}

bit_array::~bit_array()
{
    delete[] b_array;
}

bool bit_array::get(size_t i) const
{
    ASSERT(i < b_size);
    return static_cast<bool>(
            (b_array[i >> type_info<unsigned>::pwr2_length] >>
                                                           (i & static_cast<size_t>(type_info<unsigned>::bit_length - 1))) & 1u);
}

bool bit_array::operator[](size_t i) const
{
    return get(i);
}

void bit_array::clear(size_t i)
{
    ASSERT(i < b_size);
    b_array[i >> type_info<unsigned>::pwr2_length] &=
            ~(1u << (i & static_cast<size_t>(type_info<unsigned>::bit_length - 1)));
}

void bit_array::set(size_t i)
{
    ASSERT(i < b_size);
    b_array[i >> type_info<unsigned>::pwr2_length] |=
            (1u << (i & static_cast<size_t>(type_info<unsigned>::bit_length - 1)));
}

void bit_array::set(size_t i, bool f)
{
    ASSERT(i < b_size);
    if(f)
        set(i);
    else
        clear(i);
}

void bit_array::clear()
{
    _fill(b_array, get_bit_array_size(b_size));
}

size_t bit_array::size() const
{
    return b_size;
}