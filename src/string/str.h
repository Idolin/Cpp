#pragma once

#include "../debug/def_debug.h"
#include "../other/defdef.h"
#include "../template/arraymethods.hpp"
#include "../container/vector.hpp"
#include "../template/displaymethods.hpp"
#include "../container/const_array.hpp"

#include <string>

struct str
{
protected:
    struct str_info
    {
        union
        {
            char *block;
            str_info *lpart;
        };
        unsigned long len, links;

        str_info(char *s, unsigned long len) noexcept;
        str_info(str_info *lpart, unsigned long len);

        virtual ~str_info();

        virtual char operator[](unsigned long) const;
        virtual void copy_to_array(char *dst) const;
        virtual void copy_to_array(char *dst, unsigned long from, unsigned long to) const;
        virtual bool is_owner() const;
    };

    struct str_info_subs: str_info
    {
        str_info *parent;

        str_info_subs(str_info *parent, unsigned long offset, unsigned long len);

        ~str_info_subs() override;

        char operator[](unsigned long) const override;
        bool is_owner() const override;
    };

    struct str_info_cnct_char: str_info
    {
        unsigned long size;

        str_info_cnct_char(str_info*);

        void operator+=(char c);
    };

    struct str_info_cnct: str_info //for faster +=
    {
        str_info *rpart;

        str_info_cnct(str_info*, str_info*);

        ~str_info_cnct() override;

        char operator[](unsigned long) const override;
        void copy_to_array(char *dst) const override;
        void copy_to_array(char *dst, unsigned long from, unsigned long to) const override;
        bool is_owner() const override;
    };

    char *s;
    str_info *info;

    static str_info& empty();

    struct const_iterator: public std::iterator<std::random_access_iterator_tag, const char>
    {
        friend struct str;

    private:
        const str& s;
        unsigned long i;

    public:
        const_iterator(const str& s, unsigned long i = 0);

        const_iterator(const const_iterator& otr);

        bool operator==(const const_iterator& otr) const;

        bool operator!=(const const_iterator& otr) const;

        bool operator<(const const_iterator& otr) const;

        bool operator<=(const const_iterator& otr) const;

        bool operator>(const const_iterator& otr) const;

        bool operator>=(const const_iterator& otr) const;

        const_iterator& operator++();

        const_iterator operator++(int);

        const_iterator& operator--();

        const_iterator operator--(int);

        const_iterator& operator+=(unsigned long p);

        const_iterator& operator-=(unsigned long p);

        const char operator[](unsigned long index) const;

        const char operator*() const;

        const str& operator->() const;
    };
public:
    str();

    str(const char *); // NOLINT

    str(const char*, unsigned long);

    str(char *); // NOLINT

    str(char*, unsigned long);

    str(const std::string &); // NOLINT

    str(const str&);

    str(std::string&&); // NOLINT

    str(str &&) noexcept;

protected:

    str(str_info *parent, unsigned long from, unsigned long to);

public:

    ~str();

    str& operator=(const str&);

    str& operator=(str&&) noexcept;

    char at(unsigned long) const; //use this for fast access(otherwise if you call [] operator from
    // non-const context non-const [] will be executed what may lead to slowdowns and increased memory usage

    char operator[](unsigned long) const;

    char &operator[](unsigned long);

    str& operator+=(const str&);

    str& operator+=(char c);

    str& operator*=(unsigned times);

    bool operator==(const str&) const;

    bool operator!=(const str&) const;

    unsigned long length() const;

    const_array<char> c_str() const;

    const char *c_str();

    operator char*() const;

    operator std::string() const;

    str copy() const;

    str invert() const;

    template<bool copy_sub = false>
    str& compact();

    str operator()(unsigned long, unsigned long) const;

    str operator()(unsigned long) const;

    str subStr(unsigned long, unsigned long) const;

    str subStr(unsigned long) const;

    bool startswith(const str&) const;

    bool endswith(const str&) const;

    const_iterator begin() const;

    const_iterator end() const;

protected:

    void unlink() const noexcept;

};

str operator+(str a, const str& b);
str operator*(str a, unsigned times);

bool check_eof();

str read_str();

inline bool operator==(const str& a, const char *const b)
{
    for(unsigned i = 0; i < a.length(); i++)
        if(a[i] != b[i])
            return false;
    return (b[a.length()] == '\0');
}

inline bool operator==(const char *const a, const str& b)
{
    return (b == a);
}

inline bool operator==(const str &a, char *const b)
{
    return (a == static_cast<const char* const>(b));
}

inline bool operator==(char *const a, const str& b)
{
    return (b == a);
}

inline bool operator!=(const str& a, const char *const b)
{
    return not(a == b);
}

inline bool operator!=(const char *const a, const str& b)
{
    return not(b == a);
}

inline bool operator!=(const str& a, char *const b)
{
    return not(a != static_cast<const char* const>(b));
}

inline bool operator!=(char *const a, const str& b)
{
    return (b != a);
}

inline bool operator==(const str& a, const std::string& b)
{
    if(a.length() != b.length())
        return false;
    for(unsigned i = 0; i < a.length(); i++)
        if(a[i] != b[i])
            return false;
    return true;
}

inline bool operator==(const std::string& a, const str& b)
{
    return (b == a);
}

inline bool operator!=(const str& a, const std::string& b)
{
    return not(a == b);
}

inline bool operator!=(const std::string& a, const str& b)
{
    return not(b == a);
}

template<>
inline void _tshow(const str& x)
{
    printf("%s", static_cast<const char*>(x));
}