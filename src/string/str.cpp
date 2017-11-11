#include "str.h"

str::str_info::str_info(unsigned len) : len(len), links(1)
{}

str::str() : s(nullptr), info(new str_info()), zero_end(1)
{}

str::str(const char *const s) : zero_end(1)
{
    unsigned len = 0;
    while(s[len] != '\0')
        len++;
    info = new str_info(len);
    this->s = new char[len + 1];
    _copy(this->s, len + 1, s);
}

str::str(const char *const s, unsigned len) : s(new char[len + 1]),
                                              info(new str_info(len)), zero_end(1)
{
    _copy(this->s, len, s);
    this->s[len] = '\0';
}

str::str(char *const s) : s(s), zero_end(1)
{
    unsigned len = 0;
    while(s[len] != '\0')
        len++;
    info = new str_info(len);
}

str::str(char *const s, unsigned len) : s(s), info(new str_info(len)),
                                        zero_end(static_cast<unsigned char>(s[len] == '\0'))
{}

str::str(std::string &s) : str(s.c_str())
{}

str::str(const str &s) : s(s.s), info(s.info), zero_end(s.zero_end)
{
    info->links++;
}

str::~str()
{
    unlink();
}

str &str::operator=(const str &b)
{
    if(s != b.s)
    {
        unlink();
        s = b.s;
        info = b.info;
        info->links++;
        zero_end = b.zero_end;
    }
    return *this;
}

char str::operator[](unsigned i) const
{
    ASSERT(i < info->len);
    return s[i];
}

char str::operator[](unsigned i)
{
    ASSERT(i < info->len);
    return s[i];
}

void str::operator+=(const str &b)
{
    unsigned new_len = info->len + b.length();
    char *a = nullptr;
    zero_end = 1;
    if(new_len)
    {
        zero_end = 9;
        a = new char[new_len + 1];
        a[new_len] = '\0';
    }
    if(info->len)
        _copy(s, info->len, a);
    if(b.length())
        _copy(b.s, b.length(), a + info->len);
    unlink();
    s = a;
    info = new str_info(new_len);
}

bool str::operator==(const str &b) const
{
    if(info->len != b.length())
        return false;
    for(unsigned i = 0; i < info->len; i++)
        if(s[i] != b[i])
            return false;
    return true;
}

unsigned str::length() const
{
    return info->len;
}

char *&str::c_str()
{
    if(!zero_end)
    {
        char *s_new = new char[info->len + 1];
        _copy(s_new, info->len, s);
        s_new[info->len] = '\0';
        unlink();
        s = s_new;
        info->links = 1;
        zero_end = true;
    }
    return s;
}

str str::copy() const
{
    char *s_new = new char[info->len + 1];
    _copy(s_new, info->len, s);
    s_new[info->len] = '\0';
    return str(s_new, info->len);
}

str str::invert() const
{
    char *s_inv = new char[info->len + 1];
    for(unsigned i = 0; i < info->len; i++)
        s_inv[i] = s[info->len - i - 1];
    s_inv[info->len] = '\0';
    return str(s_inv, info->len);
}

const str str::subStr(unsigned l, unsigned r) const
{
    str a = str(s + l, r - l);
    a.info->links = 0;
    return a;
}

str::operator const char *() const
{
    if(zero_end)
        return s;
    char *a = new char[info->len + 1];
    _copy(a, info->len, s);
    a[info->len] = '\0';
    return a;
}

str::operator char *() const
{
    char *a = new char[info->len + 1];
    _copy(a, info->len, s);
    a[info->len] = '\0';
    return a;
}

void str::unlink() const
{
    if(info->links-- == 0)
        delete info;
    elif(info->links == 0)
    {
        delete[] s;
        delete info;
    }
}

str read_str()
{
    vect<char> s(100);
    int c;
    unsigned i = 0;
    while(((c = getchar()) != -1) && (c != '\n'))
        s[i++] = (char) c;
    s[i] = '\0';
    return str(s, i);
}
