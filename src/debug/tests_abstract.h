#pragma once

#include "../other/term.h"
#include "../container/vector.hpp"
#include "def_debug.h"

#include <exception>
#include <sys/time.h>
#include <ctime>

void ftimeprint(FILE *SOUT, unsigned long long time, bool is_ms);

namespace _test_abstract_class_
{

    struct _test_class_abstract
    {
        char *test_name;
        unsigned test_repeat_amount, max_error_amount_show, errors_to_stop, errors_occured, level;
        unsigned long long test_time_ms_or_clks;
        bool test_ok, exception_expected, exception_occured;

        explicit _test_class_abstract(char *test_name);

        virtual ~_test_class_abstract() = default;

        bool run(bool count_ms_not_clocks);

        virtual void test_body() = 0;
    };

};

struct _test_pack_class_abstract
{
    char *test_pack_name;
    bool test_ok, count_ms_not_clocks;
    unsigned errors_to_stop, errors_occured;
    vect<_test_abstract_class_::_test_class_abstract *> test_classes;
    vect<unsigned> test_failed;
    unsigned long long test_time_ms_or_clks;

    explicit _test_pack_class_abstract(char *test_pack_name);

    virtual ~_test_pack_class_abstract() = default;

    virtual bool test();
};
