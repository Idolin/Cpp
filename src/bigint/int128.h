#pragma once

#ifdef E__SIZEOF_INT128__
    using int128_t = __int128_t;
    using uint128_t = __uint128_t;
#else
    #include <iostream>
    #include <string>

    #include "../other/common.hpp"
    #include "../struct_algo/binary_search.hpp"
    
    struct int128_t
    {};
    
    struct uint128_t
    {
        uint64_t p_high, p_low;
        
        uint128_t();

        uint128_t(uint64_t);
        
        uint128_t(uint64_t, uint64_t);
        
        uint128_t operator=(uint64_t);

        uint128_t operator+=(uint64_t);
        
        uint128_t operator+=(uint128_t);
        
        uint128_t operator-=(uint64_t);
        
        uint128_t operator-=(uint128_t);
        
        uint128_t operator+() const;
        
        uint128_t operator-() const;
        
        uint128_t operator++();
        
        uint128_t operator++(int);
        
        uint128_t operator--();
        
        uint128_t operator--(int);
        
        uint128_t operator*=(uint64_t);
        
        uint128_t operator*=(uint128_t);

        uint128_t udivmod(uint64_t divisor, uint64_t *remainder = nullptr);
        
        uint128_t udivmod(uint128_t divisor, uint128_t *remainder = nullptr);

        uint128_t operator/=(uint64_t);
        
        uint128_t operator/=(uint128_t);
        
        uint128_t operator%=(uint64_t);
        
        uint128_t operator%=(uint128_t);
        
        uint128_t operator<<=(unsigned);
        
        uint128_t operator>>=(unsigned);
        
        uint128_t operator&=(uint128_t);
        
        uint128_t operator|=(uint128_t);
        
        uint128_t operator^=(uint128_t);
        
        uint128_t operator~() const;
        
        bool operator<(uint128_t) const;
        
        bool operator<=(uint128_t) const;
        
        bool operator>(uint128_t) const;
        
        bool operator>=(uint128_t) const;
        
        bool operator==(uint128_t) const;
        
        bool operator!=(uint128_t) const;

        std::string to_string() const;
    };
    
    inline uint128_t operator+(uint128_t a, uint64_t b)
    {
        return a += b;
    }
    
    inline uint128_t operator+(uint64_t a, uint128_t b)
    {
        uint128_t result = b;
        result += a;
        return result;
    }
    
    inline uint128_t operator+(uint128_t a, uint128_t b)
    {
        return a += b;
    }
    
    inline uint128_t operator-(uint128_t a, uint64_t b)
    {
        return a -= b;
    }
    
    inline uint128_t operator-(uint64_t a, uint128_t b)
    {
        uint128_t result = a;
        result -= b;
        return result;
    }
    
    inline uint128_t operator-(uint128_t a, uint128_t b)
    {
        return a -= b;
    }
    
    inline uint128_t operator*(uint128_t a, uint64_t b)
    {
        return a *= b;
    }
    
    inline uint128_t operator*(uint64_t a, uint128_t b)
    {
        uint128_t result = b;
        result *= a;
        return result;
    }
    
    inline uint128_t operator*(uint128_t a, uint128_t b)
    {
        return a *= b;
    }
    
    inline uint128_t operator/(uint128_t a, uint64_t b)
    {
        return a /= b;
    }
    
    inline uint128_t operator/(uint64_t a, uint128_t b)
    {
        uint128_t result = a;
        result /= b;
        return result;
    }
    
    inline uint128_t operator/(uint128_t a, uint128_t b)
    {
        return a /= b;
    }
    
    inline uint128_t operator%(uint128_t a, uint64_t b)
    {
        return a %= b;
    }
    
    inline uint128_t operator%(uint64_t a, uint128_t b)
    {
        uint128_t result = a;
        result %= b;
        return result;
    }
    
    inline uint128_t operator%(uint128_t a, uint128_t b)
    {
        return a %= b;
    }
    
    inline uint128_t operator&(uint128_t a, uint64_t b)
    {
        return a &= b;
    }
    
    inline uint128_t operator&(uint64_t a, uint128_t b)
    {
        uint128_t result = b;
        result &= a;
        return result;
    }
    
    inline uint128_t operator&(uint128_t a, uint128_t b)
    {
        return a &= b;
    }
    
    inline uint128_t operator|(uint128_t a, uint64_t b)
    {
        return a |= b;
    }
    
    inline uint128_t operator|(uint64_t a, uint128_t b)
    {
        uint128_t result = b;
        result |= a;
        return result;
    }
    
    inline uint128_t operator|(uint128_t a, uint128_t b)
    {
        return a |= b;
    }
    
    inline uint128_t operator^(uint128_t a, uint64_t b)
    {
        return a ^= b;
    }
    
    inline uint128_t operator^(uint64_t a, uint128_t b)
    {
        uint128_t result = b;
        result ^= a;
        return result;
    }
    
    inline uint128_t operator^(uint128_t a, uint128_t b)
    {
        return a ^= b;
    }
    
    inline uint128_t operator>>(uint128_t a, unsigned b)
    {
        return a >>= b;
    }
    
    inline uint128_t operator>>(uint128_t a, uint128_t b)
    {
        if(b.p_high)
            a = 0;
        else
            a >>= b.p_low;
        return a;
    }
    
    inline uint128_t operator<<(uint128_t a, unsigned b)
    {
        return a <<= b;
    }
    
    inline uint128_t operator<<(uint128_t a, uint128_t b)
    {
        if(b.p_high)
            a = 0;
        else
            a <<= b.p_low;
        return a;
    }

    inline std::string to_string(uint128_t a)
    {
        return a.to_string();
    }

    inline std::ostream& operator<<(std::ostream &s, uint128_t a)
    {
        return s << a.to_string();
    }

#endif
