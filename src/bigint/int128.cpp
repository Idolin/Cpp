#ifndef E__SIZEOF_INT128__

#include "./int128.h"

#include <algorithm>
#include <limits>

#include "../debug/def_debug.h"

uint128_t::uint128_t()
{}

uint128_t::uint128_t(uint64_t value): p_high(0), p_low(value)
{}

uint128_t::uint128_t(uint64_t value_high, uint64_t value_low): p_high(value_high), p_low(value_low)
{}

uint128_t uint128_t::operator=(uint64_t value)
{
    p_high = 0;
    p_low = value;
    return *this;
}

uint128_t uint128_t::operator+=(uint64_t value)
{
    p_low += value;
    if(p_low < value)
        p_high++;
    return *this;
}
    
uint128_t uint128_t::operator+=(uint128_t otr)
{
    p_low += otr.p_low;
    if(p_low < otr.p_low)
        p_high++;
    p_high += otr.p_high;
    return *this;
}
    
uint128_t uint128_t::operator-=(uint64_t value)
{
    if(value > p_low)
        p_high--;
    p_low -= value;
    return *this;
}
    
uint128_t uint128_t::operator-=(uint128_t otr)
{
    if(otr.p_low > p_low)
        p_high--;
    p_low -= otr.p_low;
    p_high -= otr.p_high;
    return *this;
}

uint128_t uint128_t::operator+() const
{
    uint128_t copy = *this;
    return copy;
}

uint128_t uint128_t::operator-() const
{
    uint128_t copy = ~*this;
    return ++copy;
}

uint128_t uint128_t::operator++()
{
    if(++p_low == 0)
        p_high++;
    return *this;
}
        
uint128_t uint128_t::operator++(int)
{
    uint128_t copy = *this;
    ++*this;
    return copy;
}
        
uint128_t uint128_t::operator--()
{
    if(p_low-- == 0)
        p_high--;
    return *this;
}
        
uint128_t uint128_t::operator--(int)
{
    uint128_t copy = *this;
    --*this;
    return copy;
}
    
uint128_t uint128_t::operator*=(uint64_t value)
{
    p_high *= value;
    
    uint64_t low_pHigh = p_low >> 32;
    uint64_t value_pHigh = value >> 32;
    p_high += low_pHigh * value_pHigh;
    
    
    uint64_t low_low = static_cast<uint32_t>(p_low);
    uint64_t value_low = static_cast<uint32_t>(value);
    
    uint64_t sum = low_pHigh * value_low;
    p_high += sum >> 32;
    sum &= 0xFFFFFFFFllu;
    
    sum += low_low * value_pHigh;
    p_high += sum >> 32;
    sum <<= 32;
    
    p_low = sum + low_low * value_low;
    if(p_low < sum)
        p_high++;
    return *this;
}
    
uint128_t uint128_t::operator*=(uint128_t otr)
{
    uint64_t low_copy = p_low;
    *this *= otr.p_low;
    p_high += low_copy * otr.p_high;
    return *this;
}

uint128_t uint128_t::udivmod(uint64_t divisor, uint64_t *remainder)
{
    ASSERT(divisor != 0);
    if((divisor & (divisor - 1)) == 0) // value = 2 ^ x
    {
        unsigned shift = 0;
        if(divisor >= (UINT64_C(1) << 32))
            shift = 32;
        if(divisor >= (UINT64_C(1) << (shift + 16)))
            shift += 16;
        if(divisor >= (UINT64_C(1) << (shift + 8)))
            shift += 8;
        if(divisor >= (UINT64_C(1) << (shift + 4)))
            shift += 4;
        if(divisor >= (UINT64_C(1) << (shift + 2)))
            shift += 2;
        if(divisor >= (UINT64_C(1) << (shift + 1)))
            shift++;
        if(remainder)
            *remainder = p_low & ((UINT64_C(1) << shift) - 1);
        *this >>= shift;
        return *this;
    }

    uint64_t remainder_ = p_high;
    if(p_high >= divisor)
    {
        remainder_ = p_high % divisor;
        p_high /= divisor;
    }
    else
        p_high = 0;

    bool carry;

    for(unsigned i = 0;i < 64;i++)
    {
        // carry <- q_(64-i) bit
        carry = remainder_ >> 63; // carry <- true if remainder >= 2^64
        remainder_ <<= 1;
        remainder_ |= (p_low >> 63);
        p_low <<= 1;

        if((carry) || (remainder_ >= divisor))
        {
            remainder_ -= divisor;
            p_low |= 1;
        }
    }

    if(remainder)
        *remainder = remainder_;

    return *this;
}

uint128_t uint128_t::udivmod(uint128_t divisor, uint128_t *remainder)
{
    if(divisor.p_high == 0)
    {
        if(remainder)
        {
            remainder->p_high = 0;
            udivmod(divisor.p_low, &(remainder->p_low));
        }
        else
            udivmod(divisor.p_low);
    }
    elif(*this < divisor)
    {
        if(remainder)
            *remainder = *this;
        *this = 0;
    }
    else // result < 2^64
    {
        const uint64_t c_2p32_1 = UINT64_C(0xffffffff); // 2^32 - 1
        if(p_high <= c_2p32_1)
        {
            uint64_t dividend = (p_high << 32) | (p_low >> 32);
            uint64_t divisor_ = (divisor.p_high << 32) | (divisor.p_low >> 32);
            uint64_t quotient = dividend / divisor_; // result < 2^32
            uint64_t remainder_ = dividend % divisor_;
            if((remainder_ < c_2p32_1) &&
                    (((remainder_ << 32) + (p_low & c_2p32_1)) < (quotient * (divisor.p_low & c_2p32_1))))
                quotient--;
            if(remainder)
                *remainder = *this - divisor * quotient;
            *this = quotient;
        }
        else if(divisor.p_high > c_2p32_1)
        {

            uint64_t dividend = p_high;
            uint64_t divisor_ = divisor.p_high;
            uint64_t quotient = dividend / divisor_; // result < 2^32
            uint64_t remainder_ = dividend % divisor_;
            if(remainder_ < c_2p32_1)
            {
                remainder_ = (remainder_ << 32) + (p_low >> 32);
                uint64_t r_check = quotient * (divisor.p_low >> 32);
                if(remainder_ < r_check)
                    quotient--;
                else
                {
                    remainder_ -= r_check;
                    if((remainder_ < c_2p32_1) &&
                            (((remainder_ << 32) + (p_low & c_2p32_1)) < (quotient * (divisor.p_low & c_2p32_1))))
                        quotient--;
                }
            }
            if(remainder)
                *remainder = *this - divisor * quotient;
            *this = quotient;
        }
        else
        {
            uint64_t quotient = 0;
            uint_fast8_t shift = 0;
            uint64_t dividend_high = p_high >> 1;

            while(dividend_high >= divisor.p_high)
            {
                dividend_high >>= 1;
                shift++;
            }
            divisor <<= shift;

            while(true)
            {
                if(*this >= divisor)
                {
                    *this -= divisor;
                    quotient |= 1;
                }
                if(shift-- == 0)
                    break;
                quotient <<= 1;
                divisor >>= 1;
            }

            // *this equals to remainder before assignment
            if(remainder)
                *remainder = *this;
            *this = quotient;
        }
    }
    return *this;
}

uint128_t uint128_t::operator/=(uint64_t divisor)
{
    return udivmod(divisor);
}

uint128_t uint128_t::operator/=(uint128_t divisor)
{
    return udivmod(divisor);
}

uint128_t uint128_t::operator%=(uint64_t divisor)
{
    ASSERT(divisor != 0);
    if((divisor & (divisor - 1)) == 0) // value = 2 ^ x
        *this = p_low & (divisor - 1);
    else
    {
        uint64_t rem;
        udivmod(divisor, &rem);
        *this = rem;
    }
    return *this;
}
        
uint128_t uint128_t::operator%=(uint128_t divisor)
{
    uint128_t copy = *this;
    copy.udivmod(divisor, this);
    return *this;
}

uint128_t uint128_t::operator<<=(unsigned shift)
{
    ASSERT(shift < 128);
    if(shift >= 64)
    {
        p_high = p_low << (shift - 64);
        p_low = 0;
    }
    else if(shift)
    {
        p_high <<= shift;
        p_high |= (p_low >> (64 - shift));
        p_low <<= shift;
    }
    return *this;
}
        
uint128_t uint128_t::operator>>=(unsigned shift)
{
    ASSERT(shift < 128);
    if(shift >= 64)
    {
        p_low = p_high >> (shift - 64);
        p_high = 0;
    }
    else if(shift)
    {
        p_low >>= shift;
        p_low |= (p_high << (64 - shift));
        p_high >>= shift;
    }
    return *this;
}

uint128_t uint128_t::operator&=(uint128_t otr)
{
    p_high &= otr.p_high;
    p_low &= otr.p_low;
    return *this;
}
        
uint128_t uint128_t::operator|=(uint128_t otr)
{
    p_high |= otr.p_high;
    p_low |= otr.p_low;
    return *this;
}
        
uint128_t uint128_t::operator^=(uint128_t otr)
{
    p_high ^= otr.p_high;
    p_low ^= otr.p_low;
    return *this;
}
        
uint128_t uint128_t::operator~() const
{
    uint128_t copy = uint128_t(~p_high, ~p_low);
    return copy;
}

bool uint128_t::operator<(uint128_t otr) const
{
    return (p_high < otr.p_high) || (p_high == otr.p_high) && (p_low < otr.p_low);
}
        
bool uint128_t::operator<=(uint128_t otr) const
{
    return (p_high < otr.p_high) || (p_high == otr.p_high) && (p_low <= otr.p_low);
}

bool uint128_t::operator>(uint128_t otr) const
{
    return (p_high > otr.p_high) || (p_high == otr.p_high) && (p_low > otr.p_low);
}

bool uint128_t::operator>=(uint128_t otr) const
{
    return (p_high > otr.p_high) || (p_high == otr.p_high) && (p_low >= otr.p_low);
}

bool uint128_t::operator==(uint128_t otr) const
{
    return (p_high == otr.p_high) && (p_low == otr.p_low);
}

bool uint128_t::operator!=(uint128_t otr) const
{
    return !(*this == otr);
}

std::string uint128_t::to_string() const
{
    if(*this == 0)
        return "0";
    uint128_t copy = *this;
    std::string s;
    const uint64_t max10 = UINT64_C(10000000000000000000);
    int zero_count = 0;
    do
    {
        uint64_t rem;
        copy.udivmod(max10, &rem);
        while(zero_count-- > 0)
            s += '0';
        zero_count = 19;
        while(rem)
        {
            zero_count--;
            s += (char) (rem % 10 + '0');
            rem /= 10;
        }
    } while(copy > 0);
    std::reverse(s.begin(), s.end());
    return s;
}

#endif
