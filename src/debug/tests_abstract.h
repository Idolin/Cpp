#pragma once

#include "../other/counter.h"
#include "../other/term.h"
#include "../container/vector.hpp"
#include "def_debug.h"
#include "../string/str.h"

#include <exception>

void fprinttime(FILE *SOUT, unsigned long long milliseconds, const char *add_str = nullptr);

namespace _test_abstract_class_
{

    struct _test_class_abstract
    {
        const char *test_name;
        unsigned test_repeat_amount, max_error_amount_show, errors_to_stop,
                errors_occured;
        process_time_counter counter;
        bool test_ok, exception_expected, exception_occured;
        vect<const char*> test_with;
        vect<std::pair<const char*, process_time_counter>> subtests;
        vect<const char*> subtests_failed;

        explicit _test_class_abstract(const char *test_name);

        virtual ~_test_class_abstract() = default;

        bool run();

        virtual void test_body() = 0;

        bool print_test_with(term_color color = term_color::DEFAULT);

        void print_level(term_color color = term_color::KEEP);

        bool new_subtest(const char *subtest_name);

        bool check_subtest(bool saved);
    };

    struct _test_with_wrapper
    {
        _test_class_abstract *test_p;

        _test_with_wrapper(_test_class_abstract *p, const char *with);

        ~_test_with_wrapper();
    };
};

struct _test_pack_class_abstract
{
    const char *test_pack_name;
    bool test_ok;
    unsigned errors_to_stop, errors_occured;
    vect<_test_abstract_class_::_test_class_abstract *> test_classes;
    vect<unsigned> test_failed;
    unsigned long long milliseconds;

    explicit _test_pack_class_abstract(const char *test_pack_name);

    virtual ~_test_pack_class_abstract() = default;

    virtual bool test(str mask);
};
