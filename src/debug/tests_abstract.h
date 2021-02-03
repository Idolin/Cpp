#pragma once

#include "../other/counter.h"
#include "../other/term.h"
#include "../container/vector.hpp"
#include "def_debug.h"
#include "../string/cstr.h"
#include "../flow_processing/welford.hpp"

#include <exception>
#include <functional>

void fprinttime(FILE *SOUT, unsigned long long nanoseconds, unsigned long long tick_ns_precision);
void fprinttimer(FILE *SOUT, const welford<VARIANCE>& timer, unsigned long long tick_ns_precision = 1000, bool show_brackets=true);

namespace test_namespace
{
    struct _subtest_class;

    struct _test_class_abstract
    {
        _test_class_abstract *_this_test_ptr;
        const char *test_name;
        unsigned test_repeat_amount, max_error_amount_show, errors_to_stop,
                errors_occured, show_level;
        unsigned char counter_running;
        process_time_counter counter;
        welford<VARIANCE> time_measurement;
        bool test_ok, exception_expected, exception_occured, first_run;
        cstr test_info_str;
        vect<_test_class_abstract*> subtests;

        explicit _test_class_abstract(const char *test_name, unsigned show_level=1);

        virtual ~_test_class_abstract();

        bool run();

        virtual void test_body() = 0;
        
        virtual bool run_next();
        
        void print_test(term_color color = term_color::KEEP) const;

        void print_level(term_color color = term_color::KEEP) const;
        
        void print_failed() const;
        
        bool error(int line, const char *format, ...);
        
    private:
        bool start_counter();
        
        bool stop_counter();
    };
    
    struct _subtest_class: _test_class_abstract
    {
        std::function<void(_test_class_abstract*)> _test_body;
        
        _subtest_class(_test_class_abstract *parent, const char *subtest_name);
        
        _subtest_class(_test_class_abstract *parent, const char *subtest_name, std::function<void(_test_class_abstract*)> init);
        
        void test_body() override;
    };
    
    struct _test_pack_class
    {
        const char *test_pack_name;
        bool test_ok;
        unsigned errors_to_stop, errors_occured;
        process_time_counter counter;
        vect<_test_class_abstract*> test_classes;
        vect<unsigned> test_failed;
        unsigned long long nanoseconds_all, nanoseconds_test;

        explicit _test_pack_class(const char *test_pack_name);

        ~_test_pack_class();

        bool test(cstr mask);
        
        void print_failed() const;
    };
}
