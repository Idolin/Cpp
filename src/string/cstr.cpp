#include "cstr.h"

#include "../debug/def_debug.h"
#include "../other/singleton.hpp"

#include <cstdio>
#include <typeinfo>

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
            return cstr::not_found;
        }
    };

    template<>
    struct CFDef<2>
    {
        static unsigned long v()
        {
            return cstr::not_found;
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

        void push(unsigned long)
        {}

        unsigned long result()
        {
            return var;
        }

        unsigned long result(unsigned long)
        {
            return 0;
        }
    };

    template<>
    struct CFVT<1>
    {
        unsigned long last = cstr::not_found;

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

        void push(unsigned long)
        {}

        unsigned long result()
        {
            return cstr::not_found;
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

        vect<unsigned long>&& result(unsigned long)
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

cstr::no_copy::no_copy(char *s): s(s)
{}

cstr::to_own::to_own(char *s): s(s)
{}

cstr::const_iterator::const_iterator(const cstr& s, unsigned long i): s(s), i(i)
{}

cstr::const_iterator::const_iterator(const cstr::const_iterator& otr): s(otr.s), i(otr.i)
{}

bool cstr::const_iterator::operator==(const cstr::const_iterator& otr) const
{
    return (&s == &otr.s &&
            ((i < s.length() && i == otr.i) ||
             (i >= s.length() && otr.i >= s.length())));
}

bool cstr::const_iterator::operator!=(const cstr::const_iterator& otr) const
{
    return !(*this == otr);
}

bool cstr::const_iterator::operator<(const cstr::const_iterator& otr) const
{
    return i < otr.i;
}

bool cstr::const_iterator::operator<=(const cstr::const_iterator& otr) const
{
    return i <= otr.i;
}

bool cstr::const_iterator::operator>(const cstr::const_iterator& otr) const
{
    return i > otr.i;
}

bool cstr::const_iterator::operator>=(const cstr::const_iterator& otr) const
{
    return i >= otr.i;
}

cstr::const_iterator& cstr::const_iterator::operator++()
{
    i++;
    return *this;
}

cstr::const_iterator cstr::const_iterator::operator++(int)
{
    const_iterator copy = *this;
    ++*this;
    return copy;
}

cstr::const_iterator& cstr::const_iterator::operator--()
{
    i--;
    return *this;
}

cstr::const_iterator cstr::const_iterator::operator--(int)
{
    const_iterator copy = *this;
    --*this;
    return copy;
}

cstr::const_iterator& cstr::const_iterator::operator+=(unsigned long p)
{
    i += p;
    return *this;
}

cstr::const_iterator& cstr::const_iterator::operator-=(unsigned long p)
{
    i -= p;
    return *this;
}

char cstr::const_iterator::operator[](unsigned long index) const
{
    return s[index];
}

char cstr::const_iterator::operator*() const
{
    return s[i];
}

const cstr& cstr::const_iterator::operator->() const
{
    return s;
}

cstr::str_iterable::str_iterator::str_iterator(): s(cstr()), f(cstr()), now_index(cstr::not_found)
{}

cstr::str_iterable::str_iterator::str_iterator(const cstr &s, const cstr &f):
        s(s), f(f), now_index(0)
{
    next_index = s.find(f);
}

cstr::str_iterable::str_iterator::str_iterator(const cstr::str_iterable::str_iterator &otr):
        s(otr.s), f(otr.f), now_index(otr.now_index), next_index(otr.next_index)
{}

bool cstr::str_iterable::str_iterator::operator==(
        const cstr::str_iterable::str_iterator& otr) const
{
    return now_index == otr.now_index;
}

bool cstr::str_iterable::str_iterator::operator!=(
        const cstr::str_iterable::str_iterator& otr) const
{
    return now_index != otr.now_index;
}

cstr::str_iterable::str_iterator& cstr::str_iterable::str_iterator::operator++()
{
    now_index = next_index;
    if(now_index != cstr::not_found)
    {
        now_index += f.length();
        next_index = s.find(f, now_index);
    }
    return *this;
}

cstr::str_iterable::str_iterator cstr::str_iterable::str_iterator::operator++(int)
{
    str_iterator copy = *this;
    ++*this;
    return copy;
}

cstr cstr::str_iterable::str_iterator::operator*() const
{
    if(next_index == cstr::not_found)
        return s.substr(now_index);
    return s.substr(now_index, next_index);
}

cstr::str_iterable::str_iterator::operator bool() const
{
    return now_index != cstr::not_found;
}

cstr::str_iterable::str_iterable(const cstr &s, const cstr &p): s(s), f(p)
{}

cstr::str_iterable::str_iterator cstr::str_iterable::begin() const
{
    return cstr::str_iterable::str_iterator(s, f);
}

cstr::str_iterable::str_iterator cstr::str_iterable::end() const
{
    return cstr::str_iterable::str_iterator();
}

cstr::str_info::str_info(char *s, unsigned long len) noexcept:
        block(s), len(len), links(1), cell_changed(std::numeric_limits<unsigned long>::max())
{}

cstr::str_info::str_info(cstr::str_info *lpart, unsigned long len) noexcept:
    lpart(lpart), len(len), links(1), cell_changed(std::numeric_limits<unsigned long>::max())
{}

cstr::str_info::str_info(cstr::str_info *copy_from): HashableStored<true>(*copy_from),
    block(new char[copy_from->len + 1]), len(copy_from->len), links(1),
    cell_changed(copy_from->cell_changed)
{
    copy_array(copy_from->block, len + 1, block);
}

cstr::str_info::str_info(unsigned long new_len, str_info *copy_from):
    HashableStored<true>(*copy_from), block(new char[new_len]),
    len(copy_from->len), links(1), cell_changed(copy_from->cell_changed)
{
    copy_from->copy_to_array(block);
    fill(block + len, new_len - len);
}

cstr::str_info::~str_info()
{
    delete[] block;
}

char cstr::str_info::operator[](unsigned long i) const
{
    ASSERT(i <= len);
    return block[i];
}

void cstr::str_info::copy_to_array(char *dst) const
{
    copy_array(block, len, dst); //caller needs to care about null char in the end
}

void cstr::str_info::copy_to_array(char *dst, unsigned long from, unsigned long to) const
{
    ASSERT(from < to);
    ASSERT(to <= len);
    copy_array(block + from, to - from, dst);
}

unsigned char cstr::str_info::cannot_change() const
{
    return (links != 1);
}

void cstr::str_info::update_hash() const
{
    if(is_changed())
        set_hash(get_hash_value() + static_cast<uint64_t>(block[cell_changed]) * pow(cstr::hash_mult, cell_changed));
}

void cstr::str_info::update_hash(unsigned long cell) const
{
    cell_changed = cell;
    set_hash(get_hash_value() - static_cast<uint64_t>(block[cell_changed]) * pow(cstr::hash_mult, cell_changed));
    hash_changed();
}

uint64_t cstr::str_info::hash_recalc() const
{
    if(cell_changed < std::numeric_limits<unsigned long>::max())
        return get_hash_value() + static_cast<uint64_t>(block[cell_changed]) * pow(cstr::hash_mult, cell_changed);
    uint64_t hash_v = 0, m = cstr::hash_mult;
    for(unsigned long i = 0;i < len;i++)
        hash_v += static_cast<uint64_t>(block[i]) * m, m *= cstr::hash_mult;
    return hash_v;
}

cstr::str_info_subs::str_info_subs(cstr::str_info *parent, unsigned long offset, unsigned long len):
        str_info(parent->block + offset, len), parent(parent)
{
    ASSERT(offset + len <= parent->len);
    if(typeid(*parent) == typeid(str_info_subs))
        this->parent = static_cast<str_info_subs*>(parent)->parent;
    this->parent->links++;
}

cstr::str_info_subs::~str_info_subs()
{
    block = nullptr;
    if(--parent->links == 0)
        delete parent;
}

char cstr::str_info_subs::operator[](unsigned long i) const
{
    ASSERT(i <= len);
    if(i < len)
        return block[i];
    return '\0';
}

unsigned char cstr::str_info_subs::cannot_change() const
{
    return ((links | parent->links) != 1);
}

cstr::str_info_cnct_char::str_info_cnct_char(cstr::str_info *s):
        str_info(s->len + 10, s), size(s->len + 10)
{}

void cstr::str_info_cnct_char::operator+=(char c)
{
    block[len++] = c;
    if(len == size)
    {
        block = resize_array(block, size, size * 2);
        fill(block + size, size);
        size *= 2;
    }
    if(!is_changed() || cell_changed != std::numeric_limits<unsigned long>::max())
        set_hash(get_hash_value() + static_cast<uint64_t>(c) * pow(hash_mult, len));
}

cstr::str_info_cnct::str_info_cnct(cstr::str_info *lpart, cstr::str_info *rpart):
        str_info(lpart, lpart->len + rpart->len), rpart(rpart)
{
    rpart->links++; //lpart->links incremented by str_info constructor
}

cstr::str_info_cnct::~str_info_cnct()
{
    if(--lpart->links == 0)
        delete lpart;
    if(--rpart->links == 0)
        delete rpart;
    block = nullptr; //for ~str_info to not to try to delete str_info *lpart as char*
}

char cstr::str_info_cnct::operator[](unsigned long i) const
{
    if(i < lpart->len)
        return (*lpart)[i];
    return (*rpart)[i - lpart->len];
}

void cstr::str_info_cnct::copy_to_array(char* dst) const
{
    lpart->copy_to_array(dst);
    rpart->copy_to_array(dst + lpart->len);
}

void cstr::str_info_cnct::copy_to_array(char* dst, unsigned long from, unsigned long to) const
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

unsigned char cstr::str_info_cnct::cannot_change() const
{
    return 1;
}

uint64_t cstr::str_info_cnct::hash_recalc() const
{
    return lpart->hash() + rpart->hash() * pow(cstr::hash_mult, lpart->len);
}

cstr::str_info_pi::str_info_pi(str_info *i): str_info(i, i->len)
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

cstr::str_info_pi::~str_info_pi()
{
    if(--lpart->links == 0)
        delete lpart;
    block = nullptr;
    delete[] pi;
}

char cstr::str_info_pi::operator[](unsigned long i) const
{
    return (*lpart)[i];
}

void cstr::str_info_pi::copy_to_array(char* dst) const
{
    lpart->copy_to_array(dst);
}

void cstr::str_info_pi::copy_to_array(char* dst, unsigned long from, unsigned long to) const
{
    lpart->copy_to_array(dst, from, to);
}

unsigned char cstr::str_info_pi::cannot_change() const
{
    return 2 - lpart->cannot_change() == 0;
}

uint64_t cstr::str_info_pi::hash_recalc() const
{
    return lpart->hash();
}

cstr::cstr(): s(empty().block), info(&empty())
{
    info->links++;
}

cstr::cstr(bool f): s(new char[6]), info(new str_info(s, 4))
{
    if(f)
        s[0] = 't', s[1] = 'r', s[2] = 'u', s[3] = 'e', s[4] = '\0';
    else
    {
        s[0] = 'f', s[1] = 'a', s[2] = 'l', s[3] = 's', s[4] = 'e', s[5] = '\0';
        info->len++;
    }
}

cstr::cstr(char c): s(new char[2]), info(new str_info(s, 1))
{
    s[0] = c;
    s[1] = '\0';
}

template<typename I, typename>
cstr::cstr(I x)
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
        reverse_array(s + ((s[0] == '-')), s + l);
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

template cstr::cstr(signed char);

template cstr::cstr(unsigned char);

template cstr::cstr(short);

template cstr::cstr(unsigned short);

template cstr::cstr(int);

template cstr::cstr(unsigned int);

template cstr::cstr(long);

template cstr::cstr(unsigned long);

template cstr::cstr(long long);

template cstr::cstr(unsigned long long);

cstr::cstr(const char *s)
{
    unsigned len = 0;
    while(s[len++] != '\0');
    this->s = new_array_copy(s, len);
    info = new str_info(this->s, --len);
}

cstr::cstr(const char *s, unsigned long len)
{
    this->s = new_array_copy(s, len, len + 1);
    info = new str_info(this->s, len);
}

cstr::cstr(no_copy c): s(c.s)
{
    unsigned len = 0;
    while(s[len] != '\0')
        len++;
    info = new str_info(s, len);
    info -> links++;    //we not own this resource
}

cstr::cstr(no_copy c, unsigned long len): s(c.s)
{
    ASSERT(s[len] == '\0');
    info = new str_info(s, len);
    info -> links++;    //we not own this resource
}

cstr::cstr(to_own c): s(c.s)
{
    unsigned len = 0;
    while(s[len] != '\0')
        len++;
    info = new str_info(s, len);
}

cstr::cstr(to_own c, unsigned long len): s(c.s)
{
    ASSERT(s[len] == '\0');
    info = new str_info(s, len);
}

cstr::cstr(const std::string& s): cstr(s.c_str(), static_cast<unsigned long>(s.length()))
{}

cstr::cstr(const cstr& s): s(s.s), info(s.info)
{
    info->links++;
}

cstr::cstr(std::string&& s): cstr(s.c_str(), static_cast<unsigned long>(s.length()))
{
    s.clear();
}

cstr::cstr(cstr&& s) noexcept: s(s.s), info(s.info)
{
    s.s = empty().block;
    s.info = &empty();
    s.info->links++;
}

cstr::cstr(cstr::str_info *parent, unsigned long from, unsigned long to):
        s(parent->block + from), info(new str_info_subs(parent, from, to - from))
{}

cstr::~cstr()
{
    unlink();
}

cstr& cstr::operator=(const cstr& b)
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

cstr& cstr::operator=(cstr &&b) noexcept
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

char cstr::at(unsigned long i) const
{
    ASSERT(i <= info->len);
    if(s)
        return (i == info->len) ? '\0' : s[i];
    return (*static_cast<str_info_cnct*>(info))[i]; // NOLINT (we are sure about info type)
}

char cstr::operator[](unsigned long i) const
{
    return at(i);
}

char& cstr::operator[](unsigned long i)
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

cstr& cstr::operator+=(const cstr &b)
{
    s = nullptr;
    info = new str_info_cnct(info, b.info);
    return *this;
}

cstr& cstr::operator+=(char c)
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

cstr& cstr::operator*=(unsigned times)
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
    copy_array_n_times(new_block, info->len, times);
    unlink();
    info = new str_info(new_block, new_len);
    s = new_block;
    return *this;
}

bool cstr::operator==(const cstr &b) const
{
    if(info->len != b.length() || !this->hash_equals(b))
        return false;
    return (cmp_call(b) == 2);
}

bool cstr::operator!=(const cstr &b) const
{
    return !(*this == b);
}

bool cstr::operator<(const cstr& b) const
{
    return (cmp_call(b) == 0);
}

bool cstr::operator<=(const cstr& b) const
{
    return (cmp_call(b) != 1);
}

bool cstr::operator>(const cstr& b) const
{
    return (cmp_call(b) == 1);
}

bool cstr::operator>=(const cstr& b) const
{
    return (cmp_call(b) != 0);
}

unsigned long cstr::length() const
{
    return info->len;
}

const char *cstr::c_str() const
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

cstr::operator char*() const
{
    auto *block = new char[info->len + 1];
    info->copy_to_array(block);
    block[info->len] = '\0';
    return block;
}

cstr::operator std::string() const
{
    return std::string(this->c_str());
}

cstr cstr::copy() const
{
    auto *s_new = new char[info->len + 1];
    info->copy_to_array(s_new);
    s_new[info->len] = '\0';
    return cstr(to_own(s_new), info->len);
}

cstr cstr::invert() const
{
    auto *s_inv = new char[info->len + 1];
    for(unsigned i = 0; i < info->len; i++)
        s_inv[i] = s[info->len - i - 1];
    s_inv[info->len] = '\0';
    return cstr(to_own(s_inv), info->len);
}

template<bool copy_sub>
cstr cstr::compact() const
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

template cstr cstr::compact<false>() const;

template cstr cstr::compact<true>() const;

cstr cstr::operator()(unsigned long from, unsigned long to) const
{
    ASSERT(from <= to);
    ASSERT(to <= info->len);
    if(s)
        return cstr(info, from, to);
    else
    {
        char *new_array = new char[to - from + 1];
        new_array[to - from] = '\0';
        info->copy_to_array(new_array, from, to);
        return cstr(to_own(new_array), to - from);
    }
}

cstr cstr::operator()(unsigned long from) const
{
    return (*this)(from, info->len);
}

cstr cstr::substr(unsigned long from, unsigned long to) const
{
    return (*this)(from, to);
}

cstr cstr::substr(unsigned long from) const
{
    return (*this)(from);
}

bool cstr::startswith(const cstr& prefix) const
{
    if(prefix.length() > length())
        return false;
    return (prefix == substr(0, prefix.length()));
}

bool cstr::endswith(const cstr& suffix) const
{
    if(suffix.length() > length())
        return false;
    return (suffix == substr(length() - suffix.length()));
}

bool cstr::contains_char(char ch) const
{
    return count_r_find_char<2>(ch) != not_found;
}

unsigned long cstr::count_char(char ch) const
{
    return count_r_find_char<0>(ch);
}

unsigned long cstr::rfind_char(char ch, unsigned long to) const
{
    return count_r_find_char<1>(ch, to);
}

unsigned long cstr::find_char(char ch, unsigned long from) const
{
    return count_r_find_char<2>(ch, from);
}

vect<unsigned long> cstr::find_all_char(char ch) const
{
    return count_r_find_char<3>(ch);
}

bool cstr::contains(const cstr& o) const
{
    return count_r_find<2, false>(o) != not_found;
}

unsigned long cstr::count(const cstr& o) const
{
    return count_r_find<0, false>(o);
}

unsigned long cstr::count_intersect(const cstr& o) const
{
    return count_r_find<0, true>(o);
}

unsigned long cstr::rfind(const cstr& o, unsigned long to) const
{
    return count_r_find<1, true>(o, to);
}

unsigned long cstr::find(const cstr& o, unsigned long from) const
{
    return count_r_find<2, false>(o, from);
}

vect<unsigned long> cstr::find_all(const cstr& o) const
{
    return count_r_find<3, false>(o);
}

vect<unsigned long> cstr::find_all_intersect(const cstr& o) const
{
    return count_r_find<3, true>(o);
}

cstr::str_iterable cstr::split(const cstr& delim) const
{
    return str_iterable(*this, delim);
}

cstr::const_iterator cstr::begin() const
{
    return const_iterator(*this);
}

cstr::const_iterator cstr::end() const
{
    return const_iterator(*this, length());
}

void cstr::unlink() const
{
    if(--info->links == 0)
        delete info;
}

void cstr::unlink(str_info *inf) const
{
    if(--inf->links == 0)
        delete inf;
}

void cstr::prepare_change()
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

uint64_t cstr::hash() const noexcept
{
    return info->hash();
}

unsigned char cstr::cmp_call(const cstr &b) const
{
    compact();
    b.compact<true>();
    return cmp(s, b.s, info->len);
}

template<unsigned char count_r_find_all, typename RType>
RType cstr::count_r_find_char(char ch, unsigned long from) const
{
    CFVT<count_r_find_all> v;
    compact();
    for(unsigned long i = (count_r_find_all == 1 ? std::min(from, info->len) : from);
        (count_r_find_all == 1 && i > 0) || (count_r_find_all != 1 && i < info->len);
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

template<unsigned char count_r_find_all, bool intersect, typename RType>
RType cstr::count_r_find(const cstr &o, unsigned long from) const
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

STATIC_VAR_CONSTRUCTOR(cstr::str_info, cstr::empty, new char[1](), 0)

cstr operator+(cstr a, const cstr &b)
{
    return a += b;
}

cstr operator*(cstr a, unsigned times)
{
    return a *= times;
}

static bool eof = false;

bool check_eof()
{
    return eof;
}

cstr read_cstr()
{
    vect<char> s(101);
    int c;
    unsigned i = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        s[i++] = (char) c;
    if(c == -1)
        eof = true;
    s[i] = '\0';
    return cstr(s.toArray(), i);
}

std::ostream& operator<<(std::ostream& o, const cstr& s)
{
    o << s.c_str();
    return o;
}
