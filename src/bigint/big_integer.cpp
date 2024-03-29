#include "big_integer.h"

#include <limits>

using std::string;
using std::swap;
using std::min;
using std::max;

namespace
{

    inline uint64_t _abs(int64_t k)
    {
        if(k == std::numeric_limits<int64_t>::min())
            return static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1;
        return static_cast<uint64_t>((k < 0) ? -k : k);
    }

    inline uint32_t _abs(int k)
    {
        if(k == std::numeric_limits<int>::min())
            return static_cast<uint32_t>(std::numeric_limits<int>::max()) + 1;
        return static_cast<uint32_t>((k < 0) ? -k : k);
    }

}



big_integer_exception::big_integer_exception(const std::string& s)
{
    DEBUGMSG("Exception: %s", s.c_str());
    message = s;
}

const char* big_integer_exception::what() const noexcept
{
    return message.c_str();
}

big_integer::big_integer_container::big_integer_container(unsigned size): sign(0), size(size), max_set(0)
{
    number = new uint32_t[size];
}

big_integer::big_integer_container::big_integer_container(unsigned size, bool): big_integer_container(size)
{
    clear();
}

big_integer::big_integer_container::big_integer_container(const big_integer::big_integer_container& c):
        sign(c.sign), size(c.size), max_set(c.max_set)
{
    number = new_array_copy(c.number, c.size);
}

big_integer::big_integer_container::~big_integer_container()
{
    delete[] number;
}

uint32_t& big_integer::big_integer_container::operator[](int index)
{
    return number[index];
}

const uint32_t& big_integer::big_integer_container::operator[](int index) const
{
    return number[index];
}

void big_integer::big_integer_container::set_max_set(unsigned new_max_set)
{
    max_set = new_max_set;
}

void big_integer::big_integer_container::set_sign(signed char new_sign)
{
    sign = new_sign;
}

void big_integer::big_integer_container::set_sign()
{
    sign = -sign;
}

signed char big_integer::big_integer_container::get_sign() const
{
    return sign;
}

unsigned big_integer::big_integer_container::get_max_set() const
{
    return max_set;
}

unsigned big_integer::big_integer_container::get_size() const
{
    return size;
}

void big_integer::big_integer_container::getms()
{
    max_set++;
    while(max_set-- > 0)
        if(number[max_set])
            return;
    sign = 0;
}

void big_integer::big_integer_container::fill(unsigned from, unsigned length, unsigned value)
{
    ::fill(number + from, length, value);
}

void big_integer::big_integer_container::copy(unsigned from_index, unsigned length,
                                              const big_integer::big_integer_container& c, unsigned int c_index_from)
{
    copy_array(c.number + c_index_from, length, number + from_index);
}


void big_integer::big_integer_container::clear()
{
    ::fill(number, size);
}

void big_integer::big_integer_container::resize(unsigned new_size)
{
    if(new_size == size)
        return;
    if((new_size > size) && (new_size < size * 2))
        new_size = size * 2;
    number = ::resize_array(number, max_set + 1, new_size);
    if(new_size > max_set + 1)
        ::fill(number + max_set + 1, new_size - max_set - 1);
    size = new_size;
}

void big_integer::big_integer_container::ensure_capacity(unsigned capacity)
{
    if(capacity > size)
        resize(capacity);
}

void big_integer::big_integer_container::swap(big_integer::big_integer_container& c)
{
    std::swap(number, c.number);
    std::swap(sign, c.sign);
    std::swap(max_set, c.max_set);
    std::swap(size, c.size);
}

big_integer::big_integer(unsigned size, bool clear): _number(big_integer_container(size, clear))
{}

big_integer::big_integer(): _number(big_integer_container())
{}

big_integer::big_integer(int number): _number(big_integer_container())
{
    if(number >= 0)
    {
        _number.set_sign((number > 0));
        _number[0] = static_cast<uint32_t>(number);
    }
    else
    {
        _number.set_sign(-1);
        _number[0] = _abs(number);
    }
}

big_integer::big_integer(unsigned number): _number(big_integer_container())
{
    _number.set_sign((number > 0));
    _number[0] = (uint32_t) number;
}

big_integer::big_integer(long long number): _number(big_integer_container(2))
{
    uint64_t value;
    if(number >= 0)
    {
        _number.set_sign((number > 0));
        value = (uint64_t) number;
    }
    else
    {
        _number.set_sign(-1);
        value = _abs(static_cast<int64_t>(number));
    }
    _number[0] = (uint32_t) (value & 0xffffffff);
    _number[1] = (uint32_t) (value >> 32);
    _number.getms();
}

big_integer::big_integer(unsigned long long number): _number(big_integer_container(2))
{
    _number.set_sign((number > 0));
    _number[0] = (uint32_t) (number & 0xffffffff);
    _number[1] = (uint32_t) (number >> 32);
    _number.getms();
}

big_integer::big_integer(const big_integer& b): _number(b._number)
{}

big_integer& big_integer::operator=(const big_integer& b)
{
    if(&(this->_number) != &(b._number))
    {
        big_integer c(b);
        swap(c);
    }
    return *this;
}

big_integer::big_integer(const string& s): _number(big_integer_container(static_cast<unsigned>(s.length() / 9 + 1)))
{
    if(s.length() == 0)
        throw big_integer_exception("Empty string");
    _number.clear();
    unsigned pos = (s[0] == '-');
    if(s.length() == pos)
        throw big_integer_exception("String with only '-'");
    while(s[pos] == '0' && pos < s.length())
        pos++;
    if(pos == s.length())
        return;
    _number.set_sign(1);
    unsigned k = 1000000000;
    while(pos < s.length())
    {
        auto j = static_cast<unsigned>(s.length() - pos);
        uint32_t p;
        if(j < 9)
        {
            k = 10;
            for(p = 1; p < j; p++)
                k *= 10;
        }
        else
            j = 9;
        p = 0;
        for(; j > 0; pos++, j--)
            if(s[pos] < '0' || s[pos] > '9')
                throw big_integer_exception(string("Unexpected character: ") + s[pos]);
            else
                p = p * 10 + s[pos] - '0';
        *this *= k;
        while(p)
        {
            _number[j] += p;
            p = static_cast<uint32_t>(_number[j++] < p);
        }
        _number.set_max_set(max(_number.get_max_set() + 1, j) - 1);
    }
    if(s[0] == '-')
        _number.set_sign(-1);
}

big_integer::~big_integer() = default;

string big_integer::to_string() const
{
    if(_number.get_sign() == 0)
        return "0";
    string s;
    s.reserve((_number.get_max_set() + 1) * 9);
    big_integer a = big_integer(*this);
    int zeroCount = 0;
    while(a._number.get_sign() != 0)
    {
        int64_t l = a.div_uint(1000000000);
        l = (l < 0) ? -l : l;
        while(zeroCount-- > 0)
            s = '0' + s;
        zeroCount = 9;
        while(l)
        {
            zeroCount--;
            s = (char) (l % 10 + '0') + s;
            l /= 10;
        }
    }
    if(_number.get_sign() < 0)
        return "-" + s;
    else
        return s;
}

uint32_t& big_integer::operator[](int index)
{
    return _number[index];
}

uint32_t big_integer::operator[](int index) const
{
    return _number[index];
}

bool big_integer::operator==(const big_integer& b) const
{
    return (_comp(b) == 0);
}

bool big_integer::operator!=(const big_integer& b) const
{
    return (_comp(b) != 0);
}

bool big_integer::operator<(const big_integer& b) const
{
    return (_comp(b) < 0);
}

bool big_integer::operator>(const big_integer& b) const
{
    return (_comp(b) > 0);
}

bool big_integer::operator<=(const big_integer& b) const
{
    return (_comp(b) <= 0);
}

bool big_integer::operator>=(const big_integer& b) const
{
    return (_comp(b) >= 0);
}

big_integer& big_integer::operator>>=(int k)
{
    if(k < 0)
        return (*this <<= -k);
    if(k == 0)
        return *this;
    if((unsigned) k >= (_number.get_max_set() + 1) * 32 || _number.get_sign() == 0)
        return (*this = (_number.get_sign() < 0) ? -1 : 0);
    if(_number.get_sign() < 0)
        _inv(_number.get_max_set() + 1);
    unsigned s = k >> 5;
    unsigned l = k & 0x1f;
    if(l)
    {
        for(unsigned i = 0; i < _number.get_max_set() - s; i++)
            _number[i] = (_number[i + s] >> l) + (_number[i + s + 1] << (32 - l));
        _number[_number.get_max_set() - s] = _number[_number.get_max_set()] >> l;
        if(_number.get_sign() < 0)
            _number[_number.get_max_set() - s] |= 0xffffffff << (32 - l);
    }
    else
        for(unsigned i = 0; i <= _number.get_max_set() - s; i++)
            _number[i] = _number[i + s];
    if(_number.get_sign() < 0)
        _inv(_number.get_max_set() - s + 1);
    _number.fill(this->_number.get_max_set() + 1, s);
    _number.getms();
    return *this;
}

big_integer& big_integer::operator<<=(int k)
{
    if(k < 0)
        return (*this >>= -k);
    if(k == 0)
        return *this;
    if((unsigned) k >= (_number.get_size() - _number.get_max_set()) * 32 - 31)
        _resize(_number.get_max_set() + ((k + 31) / 32) + 1);
    unsigned s = static_cast<unsigned int>(k) >> 5;
    unsigned l = static_cast<unsigned int>(k) & 0x1f;
    if(l)
    {
        for(unsigned i = this->_number.get_max_set() + s + 1; i > s; i--)
            _number[i] = (_number[i - s] << l) + (_number[i - s - 1] >> (32 - l));
        _number[s] = _number[0] << l;
    }
    else
        for(unsigned i = _number.get_max_set() + s; i >= s; i--)
            _number[i] = _number[i - s];
    _number.fill(0, s);
    _number.set_max_set(_number.get_max_set() + s);
    _number.set_max_set(_number.get_max_set() +
                        ((_number.get_max_set() + 1) < _number.get_size() && _number[_number.get_max_set() + 1]));
    return *this;
}

big_integer& big_integer::operator&=(const big_integer& b)
{
    if(this->_number.get_sign() == 0)
        return *this;
    if(b._number.get_sign() == 0)
        return (*this = 0);
    
    unsigned max_set = b._number.get_max_set();
    if(_number.get_sign() > 0)
        set_min(max_set, _number.get_max_set());
    this->_number.ensure_capacity(max_set + 1);
    
    unsigned first_non_blank = 0, b_first_non_blank = 0;
    for(;_number[first_non_blank] == 0;first_non_blank++);
    for(;b._number[b_first_non_blank] == 0;b_first_non_blank++);
    unsigned invert_to = max(first_non_blank, max_set);
    
    if(_number.get_sign() < 0)
    {
        _number[first_non_blank]--;
        for(unsigned i = first_non_blank; i <= invert_to; i++)
            _number[i] = ~_number[i];
    }
    
     if(b_first_non_blank > first_non_blank)
        _number.fill(first_non_blank, b_first_non_blank - first_non_blank);
    
    if(b._number.get_sign() < 0)
    {
        if(b_first_non_blank <= max_set)
        {
            _number[b_first_non_blank] &= ~(b._number[b_first_non_blank] - 1);
            for(unsigned i = b_first_non_blank + 1; i <= max_set; i++)
                _number[i] &= ~b._number[i];
        }
    }
    else
    {
        for(unsigned i = b_first_non_blank; i <= max_set; i++)
            _number[i] &= b._number[i];
        
        if(_number.get_max_set() > max_set)
            _number.fill(max_set + 1, _number.get_max_set() - max_set);
        _number.set_max_set(max_set);
        _number.set_sign(1);
    }
        
    if(_number.get_sign() < 0)
    {
        set_min(first_non_blank, b_first_non_blank);
        for(;first_non_blank <= _number.get_max_set() && _number[first_non_blank] == 0;first_non_blank++);
        if(first_non_blank <= _number.get_max_set())
        {
            _number[first_non_blank]--;
            for(;first_non_blank <= invert_to;first_non_blank++)
                _number[first_non_blank] = ~_number[first_non_blank];
            if(max_set > _number.get_max_set())
                _number.set_max_set(max_set);
        }
        else
        {
            _number.ensure_capacity(first_non_blank + 1);
            _number[first_non_blank] = 1;
            _number.set_max_set(first_non_blank);
        }
    }
    _number.getms();
    
    return *this;
}

big_integer& big_integer::operator|=(const big_integer& b)
{
    if(this->_number.get_sign() == 0)
        return (*this = b);
    if(b._number.get_sign() == 0)
        return *this;
    
    unsigned max_set = b._number.get_max_set();
    if(_number.get_sign() < 0)
        set_min(max_set, _number.get_max_set());
    _number.ensure_capacity(max_set + 1);
    
    unsigned first_non_blank = 0, b_first_non_blank = 0;
    for(;_number[first_non_blank] == 0;first_non_blank++);
    for(;b._number[b_first_non_blank] == 0;b_first_non_blank++);
    unsigned invert_to = max(first_non_blank, max_set);
    
    if(_number.get_sign() < 0)
    {
        _number[first_non_blank]--;
        for(unsigned i = first_non_blank; i <= invert_to; i++)
            _number[i] = ~_number[i];
    }
    
    if(b._number.get_sign() < 0)
    {
        if(b_first_non_blank <= max_set)
        {
            _number[b_first_non_blank] |= ~(b._number[b_first_non_blank] - 1);
            for(unsigned i = b_first_non_blank + 1; i <= max_set; i++)
                _number[i] |= ~b._number[i];
        }
        if(_number.get_max_set() > max_set)
            _number.fill(max_set + 1, _number.get_max_set() - max_set);
        _number.set_max_set(max_set);
        _number.set_sign(-1);
    }
    else
        for(unsigned i = b_first_non_blank; i <= max_set; i++)
            _number[i] |= b._number[i];
        
    if(_number.get_sign() < 0)
    {
        set_min(first_non_blank, b_first_non_blank);
        for(;first_non_blank <= invert_to && _number[first_non_blank] == 0;first_non_blank++);
        if(first_non_blank <= invert_to)
        {
            _number[first_non_blank]--;
            for(;first_non_blank <= invert_to;first_non_blank++)
                _number[first_non_blank] = ~_number[first_non_blank];
        }
        _number.getms();
    }
    elif(max_set > _number.get_max_set())
        _number.set_max_set(max_set);
    
    return *this;
}

big_integer& big_integer::operator^=(const big_integer& b)
{
    
    if(b._number.get_sign() == 0)
        return *this;
    if(this->_number.get_sign() == 0)
        return (*this = b);
    
    unsigned max_set = b._number.get_max_set();
    if(max_set > _number.get_max_set())
    {
        _number.ensure_capacity(max_set + 1);
        _number.set_max_set(max_set);
    }
    unsigned first_non_blank = 0, b_first_non_blank = 0;
    for(;_number[first_non_blank] == 0;first_non_blank++);
    for(;b._number[b_first_non_blank] == 0;b_first_non_blank++);
    unsigned invert_to = max_set;
    
    if(_number.get_sign() < 0)
    {
        set_max(invert_to, first_non_blank);
        _number[first_non_blank]--;
        for(unsigned i = first_non_blank; i <= invert_to; i++)
            _number[i] = ~_number[i];
    }
    
    if(b._number.get_sign() < 0)
    {
        _number[b_first_non_blank] ^= ~(b._number[b_first_non_blank] - 1);
        unsigned i = b_first_non_blank + 1;
        for(; i <= max_set; i++)
            _number[i] ^= ~b._number[i];
        if(_number.get_sign() < 0)
            for(; i <= invert_to; i++)
                _number[i] = ~_number[i];
        _number.set_sign();
    }
    else
        for(unsigned i = b_first_non_blank; i <= max_set; i++)
            _number[i] ^= b._number[i];
        
    if(_number.get_sign() < 0)
    {
        set_min(first_non_blank, b_first_non_blank);
        for(;first_non_blank <= _number.get_max_set() && _number[first_non_blank] == 0;first_non_blank++);
        if(first_non_blank <= _number.get_max_set())
        {
            _number[first_non_blank]--;
            for(;first_non_blank <= invert_to;first_non_blank++)
                _number[first_non_blank] = ~_number[first_non_blank];
            if(max_set > _number.get_max_set())
                _number.set_max_set(max_set);
            else
                _number.getms();
        }
        else
        {
            _number.ensure_capacity(first_non_blank + 1);
            _number[first_non_blank] = 1;
            _number.set_max_set(first_non_blank);
        }
    }
    else
        _number.getms();
    
    return *this;
}

big_integer big_integer::operator~() const
{
    big_integer r(*this);
    return -r - 1;
}

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator-() const
{
    big_integer a(*this);
    a._number.set_sign();
    return a;
}


big_integer& big_integer::operator++()
{
    return (*this += 1);
}

big_integer big_integer::operator++(int)
{
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer& big_integer::operator--()
{
    return (*this -= 1);
}

big_integer big_integer::operator--(int)
{
    big_integer r = *this;
    --*this;
    return r;
}

big_integer& big_integer::operator+=(int k)
{
    if(_number.get_sign() == 0)
        _number.set_sign(-1);
    if((_number.get_sign() > 0) ^ (k > 0))
        this->_sub(_abs(k));
    else
        this->_add(_abs(k));
    return *this;
}

big_integer& big_integer::operator-=(int k)
{
    if(_number.get_sign() == 0)
        _number.set_sign(-1);
    if((_number.get_sign() > 0) ^ (k > 0))
        this->_add(_abs(k));
    else
        this->_sub(_abs(k));
    return *this;
}

big_integer& big_integer::operator+=(uint32_t k)
{
    if(_number.get_sign() == 0)
        return (*this = k);
    this->_add(k);
    return *this;
}

big_integer& big_integer::operator-=(uint32_t k)
{
    this->_sub(k);
    return *this;
}

big_integer& big_integer::operator+=(const big_integer& b)
{
    if(b._number.get_max_set() + 2 > this->_number.get_size())
        this->_resize(b._number.get_max_set() + 2);
    if(this->_number.get_sign() == 0 && b._number.get_sign() != 0)
        this->_number.set_sign(1);
    if(b._number.get_sign() * this->_number.get_sign() >= 0)
        this->_add(b._number);
    else
        this->_sub(b._number);
    return *this;
}

big_integer& big_integer::operator-=(const big_integer& b)
{
    if(b._number.get_sign() == 0)
        return *this;
    if(b._number.get_max_set() + 2 > this->_number.get_size())
        this->_resize(b._number.get_max_set() + 2);
    if(this->_number.get_sign() == 0)
        this->_number.set_sign(1);
    if((b._number.get_sign() ^ this->_number.get_sign()) == 0)
        this->_sub(b._number);
    else
        this->_add(b._number);
    return *this;
}

big_integer& big_integer::operator*=(int k)
{
    uint32_t kr = _abs(k);
    *this *= kr;
    if(k < 0)
        this->_number.set_sign();
    return *this;
}

big_integer& big_integer::operator*=(uint32_t k)
{
    if(k == 0)
        _number.clear();
    if(_number.get_max_set() + 1 == _number.get_size())
        this->_resize(_number.get_size() + 1);
    uint32_t buf = 0;
    for(unsigned i = 0; i <= _number.get_max_set(); i++)
    {
        uint64_t bf = (uint64_t) _number[i] * k;
        _number[i] = (uint32_t) (bf & 0xffffffff);
        bf >>= 32;
        _number[i] += buf;
        if(_number[i] < buf)
            bf++;
        buf = (uint32_t) bf;
    }
    if(buf)
    {
        _number.set_max_set(_number.get_max_set() + 1);
        this->_number[_number.get_max_set()] = buf;
    }
    return *this;
}

big_integer big_integer::operator*(const big_integer& b) const
{
    unsigned max_set_sum = this->_number.get_max_set() + b._number.get_max_set();
    big_integer r(max_set_sum + 2, true);
    r._number.set_sign(b._number.get_sign() * this->_number.get_sign());
    if(r._number.get_sign() == 0)
        return r;
    for(unsigned i = 0; i <= b._number.get_max_set(); i++)
    {
        uint64_t bf = 0;
        for(unsigned j = 0; j <= this->_number.get_max_set(); j++)
        {
            bf += (uint64_t) b[i] * this->_number[j] + r[i + j];
            r[i + j] = (uint32_t) (bf & 0xffffffff);
            bf >>= 32;
        }
        r[i + this->_number.get_max_set() + 1] = (uint32_t) bf;
    }
    r._number.set_max_set(max_set_sum + (r._number[max_set_sum + 1] > 0));
    return r;
}

big_integer& big_integer::operator*=(const big_integer& b)
{
    return (*this = *this * b);
}


big_integer& big_integer::operator/=(int k)
{
    if(k < 0)
        this->_number.set_sign();
    div_uint(_abs(k));
    return *this;
}

big_integer& big_integer::operator/=(const big_integer& b)
{
    big_integer l;
    return div_big(b, l);
}

int big_integer::operator%(int k) const
{
    if(k == 0)
        throw big_integer_exception("Division by zero");
    if(k == 1 || k == -1 || this->_number.get_sign() == 0)
        return 0;
    uint64_t bf = 0;
    for(int i = this->_number.get_max_set() - 1; i > 0; i--)
    {
        bf = (bf << 32) + this->_number[i];
        bf %= k;
    }
    return (int) bf * this->_number.get_sign();
}

int big_integer::operator%=(int k)
{
    int a = *this % k;
    *this = a;
    return a;
}

big_integer& big_integer::operator%=(const big_integer& b)
{
    big_integer l;
    div_big(b, l);
    return (*this = l);
}

int64_t big_integer::div_uint(uint32_t k)
{
    if(k == 0)
        throw big_integer_exception("Division by zero");
    if(k == 1 || _number.get_sign() == 0)
        return 0;
    uint64_t bf = 0;
    int64_t remainder;
    for(int i = _number.get_max_set(); i >= 0; i--)
    {
        bf = (bf << 32) + _number[i];
        _number[i] = (uint32_t) (bf / k);
        bf %= k;
    }
    remainder = static_cast<int64_t>(bf);
    if(_number.get_sign() < 0)
        remainder = -remainder;
    _number.getms();
    return remainder;
}

big_integer& big_integer::div_big(const big_integer& divisor, big_integer& remainder)
{
    if(divisor._number.get_sign() == 0) // <number> / 0 - error
        throw big_integer_exception("Division by zero");
    if((this->_number.get_max_set() < divisor._number.get_max_set()) ||
            (this->_number.get_max_set() == divisor._number.get_max_set() && _comp_abs_size_eq(divisor) < 0)) // abs(this) < abs(divisor)
    {
        remainder = std::move(*this);
        return (*this = 0);
    }
    if(divisor._number.get_max_set() == 0) // if divisor < 2^32
    {
        remainder = static_cast<long long>(div_uint(divisor[0]));
        if(divisor._number.get_sign() < 0)
            _number.set_sign();
        return *this;
    }

    remainder = 0;
    remainder._number.set_sign(this->_number.get_sign()); // till the end of the function remainder is quotient
    big_integer divisor_ = divisor;
    if(divisor._number.get_sign() < 0)
    {
        this->_number.set_sign(); // + / - = -, - / - = +
        divisor_._number.set_sign(1);
    }

    int k = 0;
    while(divisor_[divisor_._number.get_max_set()] < 0x80000000)
    {
        divisor_ <<= 1;
        k++;
    }
    *this <<= k;

    signed char sign = this->_number.get_sign();
    this->_number.set_sign(1);

    unsigned df = this->_number.get_max_set() - divisor_._number.get_max_set();
    remainder._resize(df + 1);

    remainder._number.set_max_set(df);

    big_integer m = divisor_ << (32 * df);
    if(*this < m)
        remainder._number.set_max_set(df - 1);
    else
    {
        remainder._number[df] = 1;
        *this -= m;
    }

    for(int i = df - 1;i >= 0;i--)
    {
        uint64_t t = static_cast<uint64_t>(this->_number[this->_number.get_max_set()]) << 32;
        if(this->_number.get_max_set())
            t += this->_number[this->_number.get_max_set() - 1];
        remainder._number[i] = static_cast<uint32_t>(min(t / divisor_[divisor_._number.get_max_set()], UINT64_C(0xffffffff)));
        m = divisor_ << (32 * i);
        *this -= m * remainder._number[i];
        while(*this < 0)
        {
            remainder._number[i]--;
            *this += divisor_ << (32 * i);
        }
    }
    swap(remainder);
    this->_number.set_sign(sign);
    return *this;
}

void big_integer::swap(big_integer& c)
{
    _number.swap(c._number);
}

signed char big_integer::_comp(const big_integer& b) const
{
    if(this->_number.get_sign() != b._number.get_sign())
        return static_cast<signed char>((this->_number.get_sign() < b._number.get_sign()) ? -1 : 1);
    if(this->_number.get_sign() == 0)
        return 0;
    if(this->_number.get_max_set() != b._number.get_max_set())
        return static_cast<signed char>((this->_number.get_max_set() < b._number.get_max_set()) ? -1 : 1);
    return _comp_abs_size_eq(b);
}

signed char big_integer::_comp_abs_size_eq(const big_integer& b) const
{
    for(int i = this->_number.get_max_set(); i >= 0; i--)
        if((*this)[i] != b[i])
            return static_cast<signed char>(((*this)[i] < b[i]) ? -1 : 1);
    return 0;
}

void big_integer::_add(unsigned k)
{
    _number[0] += k;
    unsigned i = 0;
    if(_number[0] < k)
    {
        while(++i < _number.get_size())
            if(++_number[i])
                break;
        if(i == _number.get_size())
        {
            _resize(_number.get_size() * 2);
            _number[i] = 1;
        }
        _number.set_max_set(max(_number.get_max_set(), i));
    }
}

void big_integer::_sub(unsigned k)
{
    if(_number[0] > k)
        _number[0] -= k;
    elif(_number.get_max_set() == 0)
    {
        _number[0] = k - _number[0];
        if(_number.get_sign() == 0)
            _number.set_sign(-1);
        if(_number[0])
            _number.set_sign();
        else
            _number.set_sign(0);
    } else
    {
        _number[0] -= k;
        unsigned i = 0;
        if(_number[0])
        {
            while(i++ < _number.get_max_set())
                if(_number[i])
                    break;
                else
                    _number[i] = UINT32_MAX;
            _number[i]--;
        }
        if((_number[i] == 0) && (i == _number.get_max_set()))
            _number.set_max_set(_number.get_max_set() - 1);
    }
}

void big_integer::_add(const big_integer_container& c)
{
    bool carry = false;
    unsigned i = 0;
    do
    {
        if(carry || (c[i]))
        {
            uint32_t check = _number[i];
            _number[i] += c[i] + carry;
            carry = (_number[i] <= check);
        }
    } while(++i <= c.get_max_set());
    while(carry)
        carry = (!(++_number[i++]));
    _number.set_max_set(max(_number.get_max_set(), i - 1));
}

void big_integer::_sub(const big_integer_container& c)
{
    bool carry = false;
    unsigned i = 0;
    do
    {
        if(carry || (c[i]))
        {
            uint32_t check = _number[i];
            _number[i] -= c[i] + carry;
            carry = (_number[i] >= check);
        }
    } while(++i <= c.get_max_set());
    while(carry && i <= _number.get_max_set())
        carry = (!(_number[i++]--));
    if(carry)
    {
        _number.set_sign();
        unsigned j = 0;
        while(!_number[j++]);
        _number[j - 1] = ~_number[j - 1] + 1;
        for(; j < i; j++)
            _number[j] = ~_number[j];
    }
    _number.getms();
}

void big_integer::_inv(unsigned to_size)
{
    if(to_size > _number.get_size())
        _resize(to_size);
    char carry = 1;
    for(unsigned i = 0; i <= _number.get_max_set(); i++)
    {
        _number[i] = ~_number[i] + carry;
        carry = (carry && (_number[i] == 0)) ? 1 : 0;
    }
    if(!carry)
    {
        _number.fill(_number.get_max_set() + 1, _number.get_size() - _number.get_max_set() - 1, '\255');
        _number.set_max_set(_number.get_size() - 1);
    }
    _number.getms();
}

void big_integer::_resize(unsigned new_size)
{
    _number.resize(new_size);
}

big_integer operator-(big_integer a, const big_integer& b)
{
    return a -= b;
}

big_integer operator+(big_integer a, const big_integer& b)
{
    return a += b;
}

big_integer operator*(big_integer a, int k)
{
    return a *= k;
}

big_integer operator*(big_integer a, uint32_t k)
{
    return a *= k;
}

big_integer operator/(big_integer a, int k)
{
    return a /= k;
}

big_integer operator/(big_integer a, const big_integer& b)
{
    return a /= b;
}

big_integer operator%(big_integer a, const big_integer& b)
{
    return a %= b;
}

big_integer operator>>(big_integer a, int k)
{
    return a >>= k;
}

big_integer operator<<(big_integer a, int k)
{
    return a <<= k;
}

big_integer operator&(big_integer a, const big_integer& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, const big_integer& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, const big_integer& b)
{
    return a ^= b;
}

string to_string(const big_integer& a)
{
    return a.to_string();
}

std::ostream& operator<<(std::ostream& s, const big_integer& a)
{
    return s << to_string(a);
}
