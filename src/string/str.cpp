#include "str.h"
#include "../other/singleton.hpp"

str::const_iterator::const_iterator(const str& s, unsigned long i) : s(s), i(i)
{}

str::const_iterator::const_iterator(const str::const_iterator& otr) : s(otr.s), i(otr.i)
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

str::str_info::str_info(char *s, unsigned long len) noexcept: block(s), len(len), links(1)
{}

str::str_info::str_info(str::str_info *lpart, unsigned long len): lpart(lpart), len(len), links(1)
{}

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
    _copy(dst, len, block); //caller needs to care about null char in the end
}

void str::str_info::copy_to_array(char *dst, unsigned long from, unsigned long to) const
{
    ASSERT(from < to);
    ASSERT(to <= len);
    _copy(dst, to - from, block + from);
}


bool str::str_info::is_owner() const
{
    return (links == 1);
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

bool str::str_info_subs::is_owner() const
{
    return ((links | parent->links) == 1);
}

str::str_info_cnct_char::str_info_cnct_char(str::str_info *s):
        str_info(new char[s->len + 1], s->len), size(s->len + 1)
{
    _copy(block, len + 1, s->block);
}

void str::str_info_cnct_char::operator+=(char c)
{
    block[len++] = c;
    if(len == size)
    {
        block = _resize(block, size, size * 2);
        _fill(block + size, size);
        size *= 2;
    }
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

void str::str_info_cnct::copy_to_array(char *dst) const
{
    lpart->copy_to_array(dst);
    rpart->copy_to_array(dst + lpart->len);
}

void str::str_info_cnct::copy_to_array(char *dst, unsigned long from, unsigned long to) const
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

bool str::str_info_cnct::is_owner() const
{
    return false;
}

str::str(): s(empty().block), info(&empty())
{
    empty().links++;
}

str::str(const char *const s)
{
    unsigned len = 0;
    while(s[len++] != '\0');
    this->s = new char[len];
    _copy(this->s, len, s);
    info = new str_info(this->s, --len);
}

str::str(const char *const s, unsigned long len)
{
    this->s = new char[len + 1];
    _copy(this->s, len, s);
    this->s[len] = '\0';
    info = new str_info(this->s, len);
}

str::str(char *const s): s(s)
{
    unsigned len = 0;
    while(s[len] != '\0')
        len++;
    info = new str_info(s, len);
}

str::str(char *const s, unsigned long len): s(s)
{
    ASSERT(s[len] == '\0');
    info = new str_info(s, len);
}

str::str(const std::string &s): str(s.c_str(), static_cast<unsigned long>(s.length()))
{}

str::str(const str &s): s(s.s), info(s.info)
{
    info->links++;
}

str::str(std::string &&s): str(s.c_str(), static_cast<unsigned long>(s.length()))
{
    s.clear();
}

str::str(str &&s) noexcept: s(s.s), info(s.info)
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

str& str::operator=(str&& b) noexcept
{
    unlink();
    info = b.info;
    s = b.s;
    b.info = &empty();
    b.s = empty().block;
    b.info->links++;
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

char &str::operator[](unsigned long i)
{
    ASSERT(i < info->len);
    if(!info->is_owner())
        *this = copy();
    return s[i];
}

str& str::operator+=(const str& b)
{
    s = nullptr;
    info = new str_info_cnct(info, b.info);
    return *this;
}

str& str::operator+=(char c)
{
    if(!s)
        info = new str_info_cnct(info, new str_info(new char[1]{c}, 1));
    else
    {
        if((info->links > 1) || (typeid(info) != typeid(str_info_cnct_char)))
            info = new str_info_cnct_char(info);
        static_cast<str_info_cnct_char*>(info)->operator+=(c); //NOLINT
        s = info->block;
    }
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
    if(info->len != b.length())
        return false;
    if(s)
    {
        for(unsigned i = 0; i < info->len; i++)
            if(s[i] != b[i])
                return false;
    }
    else
    {
        for(unsigned i = 0; i < info->len; i++)
            if((*static_cast<str_info_cnct*>(info))[i] != b[i]) // NOLINT (we are sure about info type)
                return false;
    }
    return true;
}

bool str::operator!=(const str &b) const
{
    return !(*this == b);
}

unsigned long str::length() const
{
    return info->len;
}

const_array<char> str::c_str() const
{
    if((!s) || (s[info->len] != '\0'))
    {
        char *array = new char[info -> len + 1];
        info->copy_to_array(array);
        return const_array<char>(array);
    }
    return const_array<char>(s, false);
}

const char *str::c_str()
{
    if((!s) || (s[info->len] != '\0'))
        *this = copy();
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
    return std::string(static_cast<char*>(*this));
}

str str::copy() const
{
    auto *s_new = new char[info->len + 1];
    info->copy_to_array(s_new);
    s_new[info->len] = '\0';
    return str(s_new, info->len);
}

str str::invert() const
{
    auto *s_inv = new char[info->len + 1];
    for(unsigned i = 0; i < info->len; i++)
        s_inv[i] = s[info->len - i - 1];
    s_inv[info->len] = '\0';
    return str(s_inv, info->len);
}

template<bool copy_sub>
str& str::compact()
{
    if((!s) || (copy_sub && s[info->len] != '\0'))
        *this = copy();
    return *this;
}

template str& str::compact<false>();

template str& str::compact<true>();

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
        return str(new_array, to - from);
    }
}

str str::operator()(unsigned long from) const
{
    return (*this)(from, info->len);
}

str str::subStr(unsigned long from, unsigned long to) const
{
    return (*this)(from, to);
}

str str::subStr(unsigned long from) const
{
    return (*this)(from);
}

bool str::startswith(const str& prefix) const
{
    if(prefix.length() > length())
        return false;
    return (prefix == subStr(0, prefix.length()));
}

bool str::endswith(const str& suffix) const
{
    if(suffix.length() > length())
        return false;
    return (suffix == subStr(length() - suffix.length()));
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