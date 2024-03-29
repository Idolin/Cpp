#pragma once

#include "../other/defdef.h"
#include "../template/arraymethods.hpp"
#include "../container/vector.hpp"
#include "../template/displaymethods.hpp"
#include "../template/commonmethods.hpp"
#include "../other/hash.hpp"

#include <iterator>
#include <limits>
#include <ostream>
#include <string>
#include <type_traits>

struct cstr: Hashable
{
    struct no_copy
    {
        char *s;

        explicit no_copy(char *s);
    };

    struct to_own
    {
        char *s;

        explicit to_own(char *s);
    };

protected:
    struct str_info: HashableStored<true>
    {
        union
        {
            char *block;
            str_info *lpart;
        };
        unsigned long len, links;
        mutable unsigned long cell_changed;

        str_info(char *s, unsigned long len) noexcept;
        str_info(str_info *lpart, unsigned long len) noexcept;
        str_info(str_info *copy_from);
        str_info(unsigned long new_len, str_info *copy_from);

        virtual ~str_info();

        virtual char operator[](unsigned long) const;
        virtual void copy_to_array(char *dst) const;
        virtual void copy_to_array(char *dst, unsigned long from, unsigned long to) const;
        virtual unsigned char cannot_change() const; //0 - OK, 1 - only copy, 2 - can move block

        void update_hash() const;
        void update_hash(unsigned long cell) const;
        uint64_t hash_recalc() const override;
    };

    struct str_info_subs: str_info
    {
        str_info *parent;

        str_info_subs(str_info *parent, unsigned long offset, unsigned long len);

        ~str_info_subs() override;

        char operator[](unsigned long) const override;
        unsigned char cannot_change() const override;
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
        unsigned char cannot_change() const override;

        uint64_t hash_recalc() const override;
    };

    struct str_info_pi: str_info
    {
        unsigned long *pi;

        str_info_pi(str_info*);

        ~str_info_pi() override;

        char operator[](unsigned long) const override;
        void copy_to_array(char *dst) const override;
        void copy_to_array(char *dst, unsigned long from, unsigned long to) const override;
        unsigned char cannot_change() const override;

        uint64_t hash_recalc() const override;
    };

    struct const_iterator: public std::iterator<std::random_access_iterator_tag, const char>
    {
        friend struct str;

    private:
        const cstr& s;
        unsigned long i;

    public:
        const_iterator(const cstr& s, unsigned long i = 0);

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

        char operator[](unsigned long index) const;

        char operator*() const;

        const cstr& operator->() const;
    };

    struct str_iterable;

protected:
    mutable char *s;
    mutable str_info *info;

private:
    static const uint64_t hash_mult = 137;

public:
    cstr();

    cstr(bool);

    cstr(char);

    template<typename I,
            typename = typename std::enable_if_t<std::is_integral<I>::value &&
                                                 !std::is_same<I, char>::value &&
                                                 !std::is_same<I, bool>::value>>
    cstr(I x); // NOLINT

    cstr(const char*); // NOLINT

    cstr(const char*, unsigned long);

    cstr(no_copy); // NOLINT

    cstr(no_copy, unsigned long);

    cstr(to_own); // NOLINT

    cstr(to_own, unsigned long);

    cstr(const std::string&); // NOLINT

    cstr(const cstr&);

    cstr(std::string&&); // NOLINT

    cstr(cstr&&) noexcept;

protected:

    cstr(str_info *parent, unsigned long from, unsigned long to);

public:

    ~cstr();

    cstr& operator=(const cstr&);

    cstr& operator=(cstr&&) noexcept;

    char at(unsigned long) const; //use this for fast access(otherwise if you call [] operator from
    // non-const context non-const [] will be executed what may lead to slowdowns and increased memory usage

    char operator[](unsigned long) const; //cstr[cstr.length()] == '\0'

    //cstr[cstr.length()] is exception, because of attempt of write access(use at method instead)
    char& operator[](unsigned long);

    cstr& operator+=(const cstr&);

    cstr& operator+=(char c);

    cstr& operator*=(unsigned times);

    bool operator==(const cstr&) const;

    bool operator!=(const cstr&) const;

    bool operator<(const cstr&) const;

    bool operator<=(const cstr&) const;

    bool operator>(const cstr&) const;

    bool operator>=(const cstr&) const;

    unsigned long length() const;

    const char *c_str() const;

    operator char*() const;

    operator std::string() const;

    cstr copy() const;

    cstr invert() const;

    template<bool copy_sub = false>
    cstr compact() const;

    cstr operator()(unsigned long, unsigned long) const;

    cstr operator()(unsigned long) const;

    cstr substr(unsigned long, unsigned long) const;

    cstr substr(unsigned long) const;

    bool startswith(const cstr&) const;

    bool endswith(const cstr&) const;
    
    bool contains_char(char) const;

    unsigned long count_char(char) const;

    unsigned long find_char(char, unsigned long from = 0) const;

    vect<unsigned long> find_all_char(char) const;

    unsigned long rfind_char(char, unsigned long to = cstr::last) const;

    bool contains(const cstr&) const;
    
    unsigned long count(const cstr&) const;

    unsigned long count_intersect(const cstr&) const;

    unsigned long rfind(const cstr&, unsigned long to = cstr::last) const;

    unsigned long find(const cstr&, unsigned long from = 0) const;

    vect<unsigned long> find_all(const cstr&) const;

    vect<unsigned long> find_all_intersect(const cstr&) const;

    str_iterable split(const cstr& = "\n") const;

    const_iterator begin() const;

    const_iterator end() const;

    static const unsigned long last = std::numeric_limits<unsigned long>::max();

    static const unsigned long not_found = std::numeric_limits<unsigned long>::max() - 1;

protected:
    void unlink() const;

    void unlink(str_info*) const;

    void prepare_change();

    uint64_t hash() const noexcept override;

    unsigned char cmp_call(const cstr &b) const;

    template<unsigned char count_r_find_all,
        typename RType = typename std::conditional_t<count_r_find_all == 3, vect<unsigned long>, unsigned long>>
    RType count_r_find_char(char ch, unsigned long from = 0) const;

    template<unsigned char count_r_find_all, bool intersect,
        typename RType = typename std::conditional_t<count_r_find_all == 3, vect<unsigned long>, unsigned long>>
    RType count_r_find(const cstr &o, unsigned long from = 0) const;

private:
    static str_info& empty();
};

struct cstr::str_iterable
{
    struct str_iterator: public std::iterator<std::forward_iterator_tag, cstr>
    {
        friend struct cstr;
    private:
        const cstr s, f;
        unsigned long now_index, next_index;

        str_iterator();

    public:
        str_iterator(const cstr &s, const cstr &p);

        str_iterator(const str_iterator &otr);

        bool operator==(const str_iterator &otr) const;

        bool operator!=(const str_iterator &otr) const;

        str_iterator& operator++();

        str_iterator operator++(int);

        cstr operator*() const;

        operator bool() const;
    };

    const cstr s, f;

    str_iterable(const cstr &s, const cstr &p);

    str_iterator begin() const;

    str_iterator end() const;
};

cstr operator+(cstr a, const cstr& b);
cstr operator*(cstr a, unsigned times);

bool check_eof();

cstr read_cstr();

std::ostream& operator<<(std::ostream& o, const cstr& a);

inline bool operator==(const cstr& a, const char* b)
{
    a.compact();
    for(unsigned i = 0; i < a.length(); i++)
        if(a[i] != b[i])
            return false;
    return (b[a.length()] == '\0');
}

inline bool operator==(const char* a, const cstr& b)
{
    return (b == a);
}

inline bool operator==(const cstr& a, char* b)
{
    return (a == const_cast<const char*>(b));
}

inline bool operator==(char* a, const cstr& b)
{
    return (b == a);
}

inline bool operator!=(const cstr& a, const char* b)
{
    return !(a == b);
}

inline bool operator!=(const char* a, const cstr& b)
{
    return !(b == a);
}

inline bool operator!=(const cstr& a, char* b)
{
    return !(a != const_cast<const char*>(b));
}

inline bool operator!=(char* a, const cstr& b)
{
    return (b != a);
}

inline bool operator==(const cstr& a, const std::string& b)
{
    if(a.length() != b.length())
        return false;
    a.compact();
    for(unsigned i = 0; i < a.length(); i++)
        if(a[i] != b[i])
            return false;
    return true;
}

inline bool operator==(const std::string& a, const cstr& b)
{
    return (b == a);
}

inline bool operator!=(const cstr& a, const std::string& b)
{
    return !(a == b);
}

inline bool operator!=(const std::string& a, const cstr& b)
{
    return !(b == a);
}

template<>
inline void _tshow(const cstr& x)
{
    printf("%s", static_cast<const char*>(x));
}
