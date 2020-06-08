#include "str.h"

#include "../debug/def_debug.h"
#include "../other/singleton.hpp"

#include <cstdio>

namespace
{
    template<unsigned char count_find_def_vect>
    struct CFDef;

    template<>
    struct CFDef<0>
    {
        static unsigned long v()
        {
            return 0;
        }
    };

    template<>
    struct CFDef<1>
    {
        static unsigned long v()
        {
            return str::not_found;
        }
    };

    template<>
    struct CFDef<2>
    {
        static unsigned long v()
        {
            return str::not_found;
        }
    };

    template<>
    struct CFDef<3>
    {
        static vect<unsigned long> v()
        {
            return vect<unsigned long>(0);
        }
    };

    template<unsigned char count_find_var_type>
    struct CFVT;

    template<>
    struct CFVT<0>
    {
        unsigned long var = 0;

        void inc()
        {
            var++;
        }

        void push(unsigned long i)
        {}

        unsigned long result()
        {
            return var;
        }

        unsigned long result(unsigned long i)
        {
            return 0;
        }
    };

    template<>
    struct CFVT<1>
    {
        unsigned long last = str::not_found;

        void inc()
        {}

        void push(unsigned long i)
        {
            last = i;
        }

        unsigned long result()
        {
            return last;
        }

        unsigned long result(unsigned long i)
        {
            return i;
        }
    };

    template<>
    struct CFVT<2>
    {
        void inc()
        {}

        void push(unsigned long i)
        {}

        unsigned long result()
        {
            return str::not_found;
        }

        unsigned long result(unsigned long i)
        {
            return i;
        }
    };

    template<>
    struct CFVT<3>
    {
        vect<unsigned long> var = vect<unsigned long>();

        void inc()
        {}

        void push(unsigned long i)
        {
            var.push(i);
        }

        vect<unsigned long>&& result()
        {
            return std::move(var);
        }

        vect<unsigned long>&& result(unsigned long i)
        {
            return std::move(var);
        }
    };
}

static inline unsigned char cmp(const char *s1, const char *s2, unsigned long len)
{
    for(unsigned i = 0; i < len; i++)
        if(s1[i] != s2[i]) //should work, as s2[last] is '\NUL'
            return (s1[i] > s2[i]);
    return ((s2[len] == '\0')) * 2;
}

str::no_copy::no_copy(char *s): s(s)
{}

str::to_own::to_own(char *s): s(s)
{}

str::const_iterator::const_iterator(const str& s, unsigned long i): s(s), i(i)
{}

str::const_iterator::const_iterator(const str::const_iterator& otr): s(otr.s), i(otr.i)
{}

bool str::const_iterator::operator==(const str::const_iterator& otr) const
{
    return (&s == &otr.s &&
            ((i < s.length() && i == otr.i) ||
             (i >= s.length() && otr.i >= s.length())));
}

bool str::const_iterator::operator!=(const str::const_iterator& otr) const
{
    return !(*this == otr);
}

bool str::const_iterator::operator<(const str::const_iterator& otr) const
{
    return i < otr.i;
}

bool str::const_iterator::operator<=(const str::const_iterator& otr) const
{
    return i <= otr.i;
}

bool str::const_iterator::operator>(const str::const_iterator& otr) const
{
    return i > otr.i;
}

bool str::const_iterator::operator>=(const str::const_iterator& otr) const
{
    return i >= otr.i;
}

str::const_iterator& str::const_iterator::operator++()
{
    i++;
    return *this;
}

str::const_iterator str::const_iterator::operator++(int)
{
    const_iterator copy = *this;
    ++*this;
    return copy;
}

str::const_iterator& str::const_iterator::operator--()
{
    i--;
    return *this;
}

str::const_iterator str::const_iterator::operator--(int)
{
    const_iterator copy = *this;
    --*this;
    return copy;
}

str::const_iterator& str::const_iterator::operator+=(unsigned long p)
{
    i += p;
    return *this;
}

str::const_iterator& str::const_iterator::operator-=(unsigned long p)
{
    i -= p;
    return *this;
}

const char str::const_iterator::operator[](unsigned long index) const
{
    return s[index];
}

const char str::const_iterator::operator*() const
{
    return s[i];
}

const str& str::const_iterator::operator->() const
{
    return s;
}

str::str_iterable::str_iterator::str_iterator(): s(str()), f(str()), now_index(str::not_found)
{}

str::str_iterable::str_iterator::str_iterator(const str &s, const str &f):
        s(s), f(f), now_index(0)
{
    next_index = s.find(f);
}

str::str_iterable::str_iterator::str_iterator(const str::str_iterable::str_iterator &otr):
        s(otr.s), f(otr.f), now_index(otr.now_index), next_index(otr.next_index)
{}

bool str::str_iterable::str_iterator::operator==(
        const str::str_iterable::str_iterator& otr) const
{
    return now_index == otr.now_index;
}

bool str::str_iterable::str_iterator::operator!=(
        const str::str_iterable::str_iterator& otr) const
{
    return now_index != otr.now_index;
}

str::str_iterable::str_iterator& str::str_iterable::str_iterator::operator++()
{
    now_index = next_index;
    if(now_index != str::not_found)
    {
        now_index += f.length();
        next_index = s.find(f, now_index);
    }
    return *this;
}

str::str_iterable::str_iterator str::str_iterable::str_iterator::operator++(int)
{
    str_iterator copy = *this;
    ++*this;
    return copy;
}

str str::str_iterable::str_iterator::operator*() const
{
    if(next_index == str::not_found)
        return s.substr(now_index);
    return s.substr(now_index, next_index);
}

str::str_iterable::str_iterator::operator bool() const
{
    return now_index != str::not_found;
}

str::str_iterable::str_iterable(const str &s, const str &p): s(s), f(p)
{}

str::str_iterable::str_iterator str::str_iterable::begin() const
{
    return str::str_iterable::str_iterator(s, f);
}

str::str_iterable::str_iterator str::str_iterable::end() const
{
    return str::str_iterable::str_iterator();
}

str::str_info::str_info(char *s, unsigned long len) noexcept:
        block(s), len(len), links(1), cell_changed(std::numeric_limits<unsigned long>::max())
{}

str::str_info::str_info(str::str_info *lpart, unsigned long len) noexcept:
    lpart(lpart), len(len), links(1), cell_changed(std::numeric_limits<unsigned long>::max())
{}

str::str_info::str_info(str::str_info *copy_from): HashableStored<true>(*copy_from),
    block(new char[copy_from->len + 1]), len(copy_from->len), links(1),
    cell_changed(copy_from->cell_changed)
{
    _copy(copy_from->block, len + 1, block);
}

str::str_info::str_info(unsigned long new_len, str_info *copy_from):
    HashableStored<true>(*copy_from), block(new char[new_len]),
    len(copy_from->len), links(1), cell_changed(copy_from->cell_changed)
{
    copy_from->copy_to_array(block);
    _fill(block + len, new_len - len);
}

str::str_info::~str_info()
{
    delete[] block;
}

char str::str_info::operator[](unsigned long i) const
{
    ASSERT(i <= len);
    return block[i];
}

void str::str_info::copy_to_array(char *dst) const
{
    _copy(block, len, dst); //caller needs to care about null char in the end
}

void str::str_info::copy_to_array(char *dst, unsigned long from, unsigned long to) const
{
    ASSERT(from < to);
    ASSERT(to <= len);
    _copy(block + from, to - from, dst);
}

unsigned char str::str_info::cannot_change() const
{
    return (links != 1);
}

void str::str_info::update_hash() const
{
    if(is_changed())
        set_hash(get_hash_value() + static_cast<uint64_t>(block[cell_changed]) * pow(str::hash_mult, cell_changed));
}

void str::str_info::update_hash(unsigned long cell) const
{
    cell_changed = cell;
    set_hash(get_hash_value() - static_cast<uint64_t>(block[cell_changed]) * pow(str::hash_mult, cell_changed));
    hash_changed();
}

uint64_t str::str_info::hash_recalc() const
{
    if(cell_changed < std::numeric_limits<unsigned long>::max())
        return get_hash_value() + static_cast<uint64_t>(block[cell_changed]) * pow(str::hash_mult, cell_changed);
    uint64_t hash_v = 0, m = str::hash_mult;
    for(unsigned long i = 0;i < len;i++)
        hash_v += static_cast<uint64_t>(block[i]) * m, m *= str::hash_mult;
    return hash_v;
}

str::str_info_subs::str_info_subs(str::str_info *parent, unsigned long offset, unsigned long len):
        str_info(parent->block + offset, len), parent(parent)
{
    ASSERT(offset + len <= parent->len);
    if(typeid(*parent) == typeid(str_info_subs))
        this->parent = static_cast<str_info_subs*>(parent)->parent;
    this->parent->links++;
}

str::str_info_subs::~str_info_subs()
{
    block = nullptr;
    if(--parent->links == 0)
        delete parent;
}

char str::str_info_subs::operator[](unsigned long i) const
{
    ASSERT(i <= len);
    if(i < len)
        return block[i];
    return '\0';
}

unsigned char str::str_info_subs::cannot_change() const
{
    return ((links | parent->links) != 1);
}

str::str_info_cnct_char::str_info_cnct_char(str::str_info *s):
        str_info(s->len + 10, s), size(s->len + 10)
{}

void str::str_info_cnct_char::operator+=(char c)
{
    block[len++] = c;
    if(len == size)
    {
        block = _resize(block, size, size * 2);
        _fill(block + size, size);
        size *= 2;
    }
    if(!is_changed() || cell_changed != std::numeric_limits<unsigned long>::max())
        set_hash(get_hash_value() + static_cast<uint64_t>(c) * pow(hash_mult, len));
}

str::str_info_cnct::str_info_cnct(str::str_info *lpart, str::str_info *rpart):
        str_info(lpart, lpart->len + rpart->len), rpart(rpart)
{
    rpart->links++; //lpart->links incremented by str_info constructor
}

str::str_info_cnct::~str_info_cnct()
{
    if(--lpart->links == 0)
        delete lpart;
    if(--rpart->links == 0)
        delete rpart;
    block = nullptr; //for ~str_info to not to try to delete str_info *lpart as char*
}

char str::str_info_cnct::operator[](unsigned long i) const
{
    if(i < lpart->len)
        return (*lpart)[i];
    return (*rpart)[i - lpart->len];
}

void str::str_info_cnct::copy_to_array(char* dst) const
{
    lpart->copy_to_array(dst);
    rpart->copy_to_array(dst + lpart->len);
}

void str::str_info_cnct::copy_to_array(char* dst, unsigned long from, unsigned long to) const
{
    ASSERT(from < to);
    ASSERT(to <= len);
    if(to <= lpart->len)
        lpart->copy_to_array(dst, from, to);
    elif(from < lpart->len)
    {
        lpart->copy_to_array(dst, from, lpart->len);
        rpart->copy_to_array(dst + lpart->len - from, 0, to - lpart->len);
    }
    else
        rpart->copy_to_array(dst, from - lpart->len, to - lpart->len);
}

unsigned char str::str_info_cnct::cannot_change() const
{
    return 1;
}

uint64_t str::str_info_cnct::hash_recalc() const
{
    return lpart->hash() + rpart->hash() * pow(str::hash_mult, lpart->len);
}

str::str_info_pi::str_info_pi(str_info *i): str_info(i, i->len)
{
    const char *s = lpart->block;
    pi = new unsigned long[len];
    pi[0] = 0;
    for(unsigned long i = 1, k = 0; i < len; i++)
    {
        while((k > 0) && (s[i] != s[k]))
            k = pi[k - 1];
        if(s[i] == s[k])
            k++;
        pi[i] = k;
    }
}

str::str_info_pi::~str_info_pi()
{
    if(--lpart->links == 0)
        delete lpart;
    block = nullptr;
    delete[] pi;
}

char str::str_info_pi::operator[](unsigned long i) const
{
    return (*lpart)[i];
}

void str::str_info_pi::copy_to_array(char* dst) const
{
    lpart->copy_to_array(dst);
}

void str::str_info_pi::copy_to_array(char* dst, unsigned long from, unsigned long to) const
{
    lpart->copy_to_array(dst, from, to);
}

unsigned char str::str_info_pi::cannot_change() const
{
    return 2 - lpart->cannot_change() == 0;
}

uint64_t str::str_info_pi::hash_recalc() const
{
    return lpart->hash();
}

str::str(): s(empty().block), info(&empty())
{
    empty().links++;
}

str::str(bool f): s(new char[6]), info(new str_info(s, 4))
{
    if(f)
        s[0] = 't', s[1] = 'r', s[2] = 'u', s[3] = 'e', s[4] = '\0';
    else
    {
        s[0] = 'f', s[1] = 'a', s[2] = 'l', s[3] = 's', s[4] = 'e', s[5] = '\0';
        info->len++;
    }
}

str::str(char c): s(new char[2]), info(new str_info(s, 1))
{
    s[0] = c;
    s[1] = '\0';
}

template<typename I, typename>
str::str(I x)
{
    if(x)
    {
        s = new char[std::numeric_limits<I>::digits10 + 1 + ((x < 0))];
        unsigned char l = 0;
        if(x < 0)
            s[l++] = '-';
        while(x)
        {
            s[l++] = '0' + x % 10;
            x /= 10;
        }
        _reverse(s + ((s[0] == '-')), s + l);
        s[l + 1] = '\0';
        info = new str_info(s, l);
    }
    else
    {
        s = new char[2];
        s[0] = '0';
        s[1] = '\0';
        info = new str_info(s, 1);
    }
}

template str::str<signed char, void>(signed char);

template str::str<unsigned char, void>(unsigned char);

template str::str<short, void>(short);

template str::str<unsigned short, void>(unsigned short);

template str::str<int, void>(int);

template str::str<unsigned int, void>(unsigned int);

template str::str<long, void>(long);

template str::str<unsigned long, void>(unsigned long);

template str::str<long long, void>(long long);

template str::str<unsigned long long, void>(unsigned long long);

str::str(const char *s)
{
    unsigned len = 0;
    while(s[len++] != '\0');
    this->s = _new_copy(s, len);
    info = new str_info(this->s, --len);
}

str::str(const char *s, unsigned long len)
{
    this->s = _new_copy(s, len, len + 1);
    info = new str_info(this->s, len);
}

str::str(no_copy c): s(c.s)
{
    unsigned len = 0;
    while(s[len] != '\0')
        len++;
    info = new str_info(s, len);
    info -> links++;    //we not own this resource
}

str::str(no_copy c, unsigned long len): s(c.s)
{
    ASSERT(s[len] == '\0');
    info = new str_info(s, len);
    info -> links++;    //we not own this resource
}

str::str(to_own c): s(c.s)
{
    unsigned len = 0;
    while(s[len] != '\0')
        len++;
    info = new str_info(s, len);
}

str::str(to_own c, unsigned long len): s(c.s)
{
    ASSERT(s[len] == '\0');
    info = new str_info(s, len);
}

str::str(const std::string& s): str(s.c_str(), static_cast<unsigned long>(s.length()))
{}

str::str(const str& s): s(s.s), info(s.info)
{
    info->links++;
}

str::str(std::string&& s): str(s.c_str(), static_cast<unsigned long>(s.length()))
{
    s.clear();
}

str::str(str&& s) noexcept: s(s.s), info(s.info)
{
    s.s = empty().block;
    s.info = &empty();
    s.info->links++;
}

str::str(str::str_info *parent, unsigned long from, unsigned long to):
        s(parent->block + from), info(new str_info_subs(parent, from, to - from))
{}

str::~str()
{
    unlink();
}

str& str::operator=(const str& b)
{
    if(info != b.info)
    {
        unlink();
        info = b.info;
        s = b.s;
        info->links++;
    }
    return *this;
}

str& str::operator=(str &&b) noexcept
{
    str_info *tmp = info;
    info = b.info;
    s = b.s;
    b.info = &empty();
    b.s = empty().block;
    b.info->links++;
    unlink(tmp);
    return *this;
}

char str::at(unsigned long i) const
{
    ASSERT(i <= info->len);
    if(s)
        return (i == info->len) ? static_cast<const char>('\0') : s[i];
    return (*static_cast<str_info_cnct*>(info))[i]; // NOLINT (we are sure about info type)
}

char str::operator[](unsigned long i) const
{
    return at(i);
}

char& str::operator[](unsigned long i)
{
    ASSERT(i < info->len);
    prepare_change();
    if(!info -> is_changed() || info->cell_changed != std::numeric_limits<unsigned long>::max())
    {
        if(info -> is_changed())
            info -> update_hash();
        info -> update_hash(i);
    }
    return s[i];
}

str& str::operator+=(const str &b)
{
    s = nullptr;
    info = new str_info_cnct(info, b.info);
    return *this;
}

str& str::operator+=(char c)
{
    if(typeid(info) == typeid(str_info_pi*))
    {
        auto new_info = info->lpart;
        new_info->links++;
        unlink();
        info = new_info;
    }
    if(info->links == 1 && typeid(*info) == typeid(str_info_cnct_char))
        static_cast<str_info_cnct_char*>(info)->operator+=(c); //NOLINT
    else
    {
        auto new_info = new str_info_cnct_char(info);
        new_info->operator+=(c); //NOLINT
        unlink();
        info = new_info;
    }
    s = info->block;
    return *this;
}

str& str::operator*=(unsigned times)
{
    if((times == 1) || (info->len == 0))
        return *this;
    if(times == 0)
    {
        s = empty().block;
        unlink();
        info = &empty();
        info->links++;
        return *this;
    }
    unsigned long new_len = info->len * times;
    auto *new_block = new char[new_len + 1];
    info->copy_to_array(new_block);
    new_block[new_len] = '\0';
    _mult_array(new_block, info->len, times);
    unlink();
    info = new str_info(new_block, new_len);
    s = new_block;
    return *this;
}

bool str::operator==(const str &b) const
{
    if(info->len != b.length() || !this->hash_equals(b))
        return false;
    return (cmp_call(b) == 2);
}

bool str::operator!=(const str &b) const
{
    return !(*this == b);
}

bool str::operator<(const str& b) const
{
    return (cmp_call(b) == 0);
}

bool str::operator<=(const str& b) const
{
    return (cmp_call(b) != 1);
}

bool str::operator>(const str& b) const
{
    return (cmp_call(b) == 1);
}

bool str::operator>=(const str& b) const
{
    return (cmp_call(b) != 0);
}

unsigned long str::length() const
{
    return info->len;
}

const char *str::c_str() const
{
    if((!s) || (s[info->len] != '\0'))
    {
        if(info->cannot_change() != 1)
            s[info->len] = '\0';
        else
        {
            s = new char[info->len + 1];
            info->copy_to_array(s);
            s[info->len] = '\0';
            unlink();
            info = new str_info(s, info->len);
        }
    }
    return s;
}

str::operator char*() const
{
    auto *block = new char[info->len + 1];
    info->copy_to_array(block);
    block[info->len] = '\0';
    return block;
}

str::operator std::string() const
{
    return std::string(this->c_str());
}

str str::copy() const
{
    auto *s_new = new char[info->len + 1];
    info->copy_to_array(s_new);
    s_new[info->len] = '\0';
    return str(to_own(s_new), info->len);
}

str str::invert() const
{
    auto *s_inv = new char[info->len + 1];
    for(unsigned i = 0; i < info->len; i++)
        s_inv[i] = s[info->len - i - 1];
    s_inv[info->len] = '\0';
    return str(to_own(s_inv), info->len);
}

template<bool copy_sub>
str str::compact() const
{
    if((!s) || (copy_sub && s[info->len] != '\0'))
    {
        s = new char[info->len + 1];
        info->copy_to_array(s);
        s[info->len] = '\0';
        auto new_info = new str_info(s, info->len);
        unlink();
        info = new_info;
    }
    return *this;
}

template str str::compact<false>() const;

template str str::compact<true>() const;

str str::operator()(unsigned long from, unsigned long to) const
{
    ASSERT(from <= to);
    ASSERT(to <= info->len);
    if(s)
        return str(info, from, to);
    else
    {
        char *new_array = new char[to - from + 1];
        new_array[to - from] = '\0';
        info->copy_to_array(new_array, from, to);
        return str(to_own(new_array), to - from);
    }
}

str str::operator()(unsigned long from) const
{
    return (*this)(from, info->len);
}

str str::substr(unsigned long from, unsigned long to) const
{
    return (*this)(from, to);
}

str str::substr(unsigned long from) const
{
    return (*this)(from);
}

bool str::startswith(const str& prefix) const
{
    if(prefix.length() > length())
        return false;
    return (prefix == substr(0, prefix.length()));
}

bool str::endswith(const str& suffix) const
{
    if(suffix.length() > length())
        return false;
    return (suffix == substr(length() - suffix.length()));
}

bool str::contains_char(char ch) const
{
    return count_r_find_char<2>(ch) != not_found;
}

unsigned long str::count_char(char ch) const
{
    return count_r_find_char<0>(ch);
}

unsigned long str::rfind_char(char ch, unsigned long to) const
{
    return count_r_find_char<1>(ch, to);
}

unsigned long str::find_char(char ch, unsigned long from) const
{
    return count_r_find_char<2>(ch, from);
}

vect<unsigned long> str::find_all_char(char ch) const
{
    return count_r_find_char<3>(ch);
}

bool str::contains(const str& o) const
{
    return count_r_find<2, false>(o) != not_found;
}

unsigned long str::count(const str& o) const
{
    return count_r_find<0, false>(o);
}

unsigned long str::count_intersect(const str& o) const
{
    return count_r_find<0, true>(o);
}

unsigned long str::rfind(const str& o, unsigned long to) const
{
    return count_r_find<1, true>(o, to);
}

unsigned long str::find(const str& o, unsigned long from) const
{
    return count_r_find<2, false>(o, from);
}

vect<unsigned long> str::find_all(const str& o) const
{
    return count_r_find<3, false>(o);
}

vect<unsigned long> str::find_all_intersect(const str& o) const
{
    return count_r_find<3, true>(o);
}

str::str_iterable str::split(const str& delim) const
{
    return str_iterable(*this, delim);
}

str::const_iterator str::begin() const
{
    return const_iterator(*this);
}

str::const_iterator str::end() const
{
    return const_iterator(*this, length());
}

void str::unlink() const noexcept
{
    if(--info->links == 0)
        delete info;
}

void str::unlink(str_info *inf) const noexcept
{
    if(--inf->links == 0)
        delete inf;
}

void str::prepare_change()
{
    switch(info->cannot_change())
    {
        case 1:
            *this = copy();
            break;
        case 2:
            info->block = nullptr;
            unsigned long len = info->len;
            unlink();
            info = new str_info(s, len);
    }
}

uint64_t str::hash() const noexcept
{
    return info->hash();
}

unsigned char str::cmp_call(const str &b) const
{
    compact();
    b.compact<true>();
    return cmp(s, b.s, info->len);
}

template<unsigned char count_r_find_all,
    typename RType = typename std::conditional_t<count_r_find_all == 3, vect<unsigned long>, unsigned long>>
RType str::count_r_find_char(char ch, unsigned long from) const
{
    CFVT<count_r_find_all> v;
    compact();
    for(unsigned long i = (count_r_find_all == 1 ? std::min(from, info->len) : from);
        count_r_find_all == 1 && i > 0 || count_r_find_all != 1 && i < info->len;
        i += (count_r_find_all != 1 ? 1 : 0))
        {
            i -= (count_r_find_all == 1 ? 1 : 0);
            if(s[i] == ch)
            {
                if(count_r_find_all == 0)
                    v.inc();
                elif(count_r_find_all < 3)
                    return v.result(i);
                else
                    v.push(i);
            }
        }
    return v.result();
}

template<unsigned char count_r_find_all, bool intersect,
    typename RType = typename std::conditional_t<count_r_find_all == 3, vect<unsigned long>, unsigned long>>
RType str::count_r_find(const str &o, unsigned long from) const
{
    if(o.length() < 2)
    {
        if(o.length() == 0)
            return CFDef<count_r_find_all>::v();
        else
            return count_r_find_char<count_r_find_all>(o[0], from);
    }
    if(typeid(*o.info) != typeid(str_info_pi))
    {
        o.compact();
        o.info = new str_info_pi(o.info);
    }
    unsigned long *pi = static_cast<str_info_pi*>(o.info)->pi;
    CFVT<count_r_find_all> v;
    compact();
    for(unsigned long i = (count_r_find_all == 1 ? 0 : from), k = 0;
        i < (count_r_find_all == 1 ? std::min(from, info->len) : info->len); i++)
    {
        if((k > 0) && (s[i] != o.s[k]))
        {
            do
            {
                k = pi[k - 1];
            } while((k > 0) && (s[i] != o.s[k]));
            if(i + o.length() - k > (count_r_find_all == 1 ? from : info->len))
                break;
        }
        if(s[i] == o.s[k])
            if(++k == o.length())
            {
                if(count_r_find_all == 0)
                    v.inc();
                elif(count_r_find_all == 2)
                    return v.result(i + 1 - o.length());
                else
                    v.push(i + 1 - o.length());
                k = intersect ? pi[k - 1] : 0;
            }
    }
    return v.result();
}

STATIC_VAR_CONSTRUCTOR(str::str_info, str::empty, new char[1](), 0)

str operator+(str a, const str &b)
{
    return a += b;
}

str operator*(str a, unsigned times)
{
    return a *= times;
}

static bool eof = false;

bool check_eof()
{
    return eof;
}

str read_str()
{
    vect<char> s(101);
    int c;
    unsigned i = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        s[i++] = (char) c;
    if(c == -1)
        eof = true;
    s[i] = '\0';
    return str(s.toArray(), i);
}

std::ostream &operator<<(std::ostream &o, const str &s)
{
    return o << s.c_str();
}
