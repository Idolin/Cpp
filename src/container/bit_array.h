//
// Created by cynder on 21.06.19.
//

#pragma once

#include <cstddef>

struct bit_array
{
private:
    unsigned *b_array;
    size_t b_size;

public:
    explicit bit_array(size_t size);

    ~bit_array();

    bool get(size_t i) const;

    bool operator[](size_t i) const;

    void clear(size_t i);

    void set(size_t i);

    void set(size_t i, bool f);

    void clear();

    size_t size() const;
};