#pragma once

#include "../debug/def_debug.h"
#include "../other/defdef.h"
#include "../other/arraymethods.hpp"
#include "../container/vector.hpp"

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

    static str_info empty;
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

    str &operator=(str &&) noexcept;

    const char at(unsigned long) const; //use this for fast access(otherwise if you call [] operator from
    // non-const context non-const [] will be executed what may lead to slowdowns and increased memory usage

    const char operator[](unsigned long) const;

    char &operator[](unsigned long);

    str& operator+=(const str &);

    str& operator*=(unsigned times);

    bool operator==(const str &) const;

    bool operator!=(const str &) const;

    unsigned long length() const;

    const char *const c_str() const;

    const char *const c_str();

    operator char*() const; // NOLINT

    operator std::string() const; // NOLINT

    str copy() const;

    str invert() const;

    template<bool copy_sub = false>
    str& compact();

    str operator()(unsigned long, unsigned long) const;

    str operator()(unsigned long) const;

    str subStr(unsigned long, unsigned long) const;

    str subStr(unsigned long) const;

protected:

    void unlink() const noexcept;

};

str operator+(str a, const str &b);
str operator*(str a, unsigned times);

str read_str();

inline bool operator==(const str &a, const char *const b)
{
    for(unsigned i = 0; i < a.length(); i++)
        if(a[i] != b[i])
            return false;
    return (b[a.length()] == '\0');
}

inline bool operator==(const char *const a, const str &b)
{
    return (b == a);
}

inline bool operator!=(const str &a, const char *const b)
{
    return not(a == b);
}

inline bool operator!=(const char *const a, const str &b)
{
    return not(b == a);
}

inline bool operator==(const str &a, const std::string &b)
{
    if(a.length() != b.length())
        return false;
    for(unsigned i = 0; i < a.length(); i++)
        if(a[i] != b[i])
            return false;
    return true;
}

inline bool operator==(const std::string &a, const str &b)
{
    return (b == a);
}

inline bool operator!=(const str &a, const std::string &b)
{
    return not(a == b);
}

inline bool operator!=(const std::string &a, const str &b)
{
    return not(b == a);
}