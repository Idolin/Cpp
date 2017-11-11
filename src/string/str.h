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
        unsigned len, links;

        explicit str_info(unsigned = 0);
    };

    char *s;
    str_info *info;
    unsigned char zero_end;
public:
    str();

    str(const char *const);

    str(const char *const, unsigned);

    str(char *const);

    str(char *const, unsigned);

    str(std::string &);

    str(str const &);

    ~str();

    str &operator=(const str &);

    char operator[](unsigned) const;

    char operator[](unsigned);

    void operator+=(const str &);

    bool operator==(const str &) const;

    unsigned length() const;

    char *&c_str();

    str copy() const;

    str invert() const;

    const str subStr(unsigned, unsigned) const;

    operator const char *() const;

    operator char *() const;

protected:
    void unlink() const;
};

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
