#pragma once

#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <climits>
#include <exception>

#include "../template/arraymethods.hpp"
#include "../other/common.hpp"

class big_integer_exception: public std::exception
{
public:
    big_integer_exception(const std::string& s);

    virtual const char* what() const noexcept;

private:
    std::string message;
};

struct big_integer
{
private:
    struct big_integer_container
    {
    private:
        uint32_t *number;
        signed char sign;
        unsigned size, max_set; // _number[max_set] - last written ceil
    public:
        explicit big_integer_container(unsigned size = 1);
        big_integer_container(unsigned size, bool clear);
        big_integer_container(const big_integer_container& c);
        ~big_integer_container();

        uint32_t& operator[](int index);

        const uint32_t& operator[](int index) const;
        void set_max_set(unsigned new_max_set);
        void set_sign(signed char new_sign);
        void set_sign();
        signed char get_sign() const;
        unsigned get_max_set() const;
        unsigned get_size() const;
        // setting right max_set
        void getms();
        void fill(unsigned from_index, unsigned length, unsigned value = '\0');

        void copy(unsigned from_index, unsigned length, const big_integer_container& c, unsigned c_index_from);

        // Set number in container to 0.
        void clear();
        void resize(unsigned int new_size);
        void ensure_capacity(unsigned int capacity);
        void swap(big_integer_container& c);
    };
    big_integer_container _number;
private:
    big_integer(unsigned, bool);
public:
    big_integer();
    big_integer(int number);
    big_integer(unsigned number);
    big_integer(long long number);
    big_integer(unsigned long long number);
    big_integer(const big_integer& b);

    big_integer &operator=(const big_integer& b);

    explicit big_integer(const std::string& s);
    ~big_integer();
    std::string to_string() const;
private:
    uint32_t& operator[](int index);
public:
    uint32_t operator[](int index) const;

    bool operator==(const big_integer& b) const;
    bool operator!=(const big_integer& b) const;
    bool operator<(const big_integer& b) const;
    bool operator>(const big_integer& b) const;
    bool operator<=(const big_integer& b) const;
    bool operator>=(const big_integer& b) const;

    big_integer& operator>>=(int k);

    big_integer& operator<<=(int k);

    big_integer& operator&=(const big_integer& b);

    big_integer& operator|=(const big_integer& b);

    big_integer& operator^=(const big_integer& b);
    big_integer operator~() const;
    big_integer operator+() const;
    big_integer operator-() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    big_integer& operator+=(int k);

    big_integer& operator-=(int k);

    big_integer& operator+=(uint32_t k);

    big_integer& operator-=(uint32_t k);

    big_integer& operator+=(const big_integer& b);

    big_integer& operator-=(const big_integer& b);

    big_integer& operator*=(int k);

    big_integer& operator*=(uint32_t k);
    big_integer operator*(const big_integer& b) const;

    big_integer& operator*=(const big_integer& b);

    big_integer& operator/=(int k);

    big_integer& operator/=(const big_integer& b);
    int operator%(int k) const;
    int operator%=(int k);

    big_integer& operator%=(const big_integer& b);
    void swap(big_integer& c);

private:
    int64_t div_uint(uint32_t k);

    big_integer& div_big(const big_integer& b, big_integer& l);
    signed char _comp(const big_integer& b) const;
    signed char _comp_abs_size_eq(const big_integer& b) const;
    void _add(unsigned k);
    void _sub(unsigned k);

    void _add(const big_integer_container& c);

    void _sub(const big_integer_container& c);
    void _inv(unsigned to_size);
    void _resize(unsigned new_size);

    friend std::string to_string(big_integer const& a);
};

big_integer operator-(big_integer a, const big_integer& b);

big_integer operator+(big_integer a, const big_integer& b);
big_integer operator*(big_integer a, int k);
big_integer operator*(big_integer a, uint32_t k);
big_integer operator/(big_integer a, int k);

big_integer operator/(big_integer a, const big_integer& b);

big_integer operator%(big_integer a, const big_integer& b);
big_integer operator>>(big_integer a, int k);
big_integer operator<<(big_integer a, int k);

big_integer operator&(big_integer a, const big_integer& b);

big_integer operator|(big_integer a, const big_integer& b);

big_integer operator^(big_integer a, const big_integer& b);

std::string to_string(const big_integer& a);

std::ostream& operator<<(std::ostream& s, const big_integer& a);
