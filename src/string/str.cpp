#include "str.h"

str::str_info::str_info(char *s, unsigned long len) noexcept: block(s), len(len), links(1)
{}

str::str_info::str_info(str::str_info *lpart, unsigned long len): lpart(lpart), len(len), links(1)
{}

char str::str_info::operator[](unsigned long i) const
{
    ASSERT(i <= len);
    return block[i];
}

void str::str_info::copy_to_array(char *dst) const
{
    _copy(dst, len, block); //caller needs to care about null char in the end
}

str::str_info::~str_info()
{
    delete [] block;
}

str::str_info_subs::str_info_subs(str::str_info *parent, unsigned long offset, unsigned long len):
        str_info(parent->block + offset, len), parent(parent)
{
    ASSERT(offset + len <= parent->len);
    parent->links++;
}

char str::str_info_subs::operator[](unsigned long i) const
{
    ASSERT(i <= len);
    if(i < len)
        return block[i];
    return '\0';
}

str::str_info_subs::~str_info_subs()
{
    block = nullptr;
    if(--parent->links == 0)
        delete parent;
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
    block = nullptr;
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

str::str(): s(empty.block), info(&empty)
{
    empty.links++;
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

str::str(const std::string &s): str(s.c_str(), s.length())
{}

str::str(const str &s): s(s.s), info(s.info)
{
    info->links++;
}

str::str(std::string &&s): str(s.c_str(), static_cast<unsigned>(s.length()))
{
    s.clear();
}

str::str(str &&s) noexcept: s(s.s), info(s.info)
{
    s.s = empty.block;
    s.info = &empty;
    s.info->links++;
}

str::str(str::str_info *parent, unsigned long from, unsigned long to):
        s(parent->block + from), info(new str_info_subs(parent, from, to - from))
{}

str::~str()
{
    unlink();
}

str& str::operator=(const str &b)
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

str &str::operator=(str &&b) noexcept
{
    unlink();
    info = b.info;
    s = b.s;
    b.info = &empty;
    b.s = empty.block;
    b.info->links++;
    return *this;
}

const char str::operator[](unsigned long i) const
{
    ASSERT(i <= info->len);
    if(s)
        return (i == info->len) ? static_cast<const char>('\0') : s[i];
    return (*static_cast<str_info_cnct*>(info))[i]; // NOLINT (we are sure about info type)
}

str& str::operator+=(const str &b)
{
    s = nullptr;
    info = new str_info_cnct(info, b.info);
    return *this;
}

str& str::operator*=(unsigned times)
{
    if((times == 1) || (info->len == 0))
        return *this;
    if(times == 0)
    {
        s = empty.block;
        unlink();
        info = &empty;
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
            if((*static_cast<str_info_cnct *>(info))[i] != b[i]) // NOLINT (we are sure about info type)
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

const char *const str::c_str() const
{
    ASSERT(s);
    ASSERT(s[info->len] == '\0');
    return s;
}

const char *const str::c_str()
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
    return str(info, from, to);
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

void str::unlink() const noexcept
{
    if(--info->links == 0)
        delete info;
}

str::str_info str::empty = str::str_info(new char[1](), 0); // NOLINT (well, yes it can throw an exception... and it
// cannot be caught... very sad)

str operator+(str a, const str &b)
{
    return a += b;
}

str operator*(str a, unsigned times)
{
    return a *= times;
}

str read_str()
{
    vect<char> s(101);
    int c;
    unsigned i = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        s[i++] = (char) c;
    s[i] = '\0';
    return str(s, i);
}
