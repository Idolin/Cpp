#include "tests_abstract.h"

#include "../other/singleton.hpp"

#include <cmath>
#include <stdarg.h>

static inline unsigned long long std_deviation(unsigned long long variance)
{
    return sqrtl(variance) + .5;
}

void fprinttime(FILE *SOUT, unsigned long long nanoseconds, unsigned long long tick_ns_precision)
{
    static const char* identifier[7] = {"day", "hour", "minute", "second", "millisecond", "microsecond", "nanosecond"};
    static const unsigned coefficient[6] = {24, 60, 60, 1000, 1000, 1000};
    
    if(tick_ns_precision == 0)
        tick_ns_precision = 1;
    unsigned long long nanoseconds_reminder = nanoseconds % tick_ns_precision;
    nanoseconds -= nanoseconds_reminder;
    if(nanoseconds_reminder * 2 >= tick_ns_precision)
        nanoseconds += tick_ns_precision;

    unsigned value[7];
    unsigned start_from = 7;
    while(--start_from > 0)
    {
        value[start_from] = static_cast<unsigned>(nanoseconds % coefficient[start_from - 1]);
        if((nanoseconds /= coefficient[start_from - 1]) == 0)
            break;
    }
    value[0] = static_cast<unsigned>(nanoseconds);
    
    unsigned to = 6;
    while(to > 0 && tick_ns_precision >= coefficient[to - 1])
        tick_ns_precision /= coefficient[--to];
    unsigned max_to = start_from + 2 + (value[start_from] == 1);
    if(++to > max_to)
        to = max_to;
    
    if(start_from >= to)
        start_from = to - 1;
    for(unsigned i = start_from;i < to;i++)
    {
        fprintf(SOUT, "%u %s", value[i], identifier[i]);
        if(value[i] != 1)
            fputc('s', SOUT);
        if(i < to - 1)
            fputc(' ', SOUT);
    }
}

void fprinttimer(FILE *SOUT, const welford<VARIANCE>& timer, unsigned long long tick_ns_precision, bool show_brackets)
{
    if(show_brackets)
        fputc('[', SOUT);
    fprinttime(SOUT, timer.get_mean() * timer.get_count(), tick_ns_precision);
    if(timer.get_count() > 1)
    {
        fprintf(SOUT, "/%lu = ", timer.get_count());
        fprinttime(SOUT, timer.get_mean(), tick_ns_precision / timer.get_count());
        fputs("+-", SOUT);
        fprinttime(SOUT, std_deviation(timer.get_variance()), tick_ns_precision / timer.get_count());
    }
    if(show_brackets)
        fputs("]\n", SOUT);
}

namespace test_namespace
{
    _test_class_abstract::_test_class_abstract(const char *test_name, unsigned show_level):
            _this_test_ptr(this), test_name(test_name), test_repeat_amount(1), 
            max_error_amount_show(30), errors_to_stop(0), show_level(show_level), test_ok(true),
            exception_expected(false), first_run(true), test_info_str(), subtests()
    {}
    
    _test_class_abstract::~_test_class_abstract()
    {
        while(!subtests.empty())
            delete subtests.pop();
    }

    bool _test_class_abstract::run()
    {
        first_run = true;
        print_test(term_color::BLUE);
        fputc('\n', DEBUG_OUTPUT_STREAM);
        exception_occured = false;
        errors_occured = 0;
        bool test_ok_saved = test_ok;
        test_ok = true;
        while(!subtests.empty())
            delete subtests.pop();
        try
        {
            try
            {
                for(unsigned i = 0; i < test_repeat_amount; i++)
                {
                    start_counter();
                    test_body();
                    stop_counter();
                    first_run = false;
                    if(!subtests.empty())
                    {
                        unsigned long long nanoseconds = 0;
                        for(_test_class_abstract *subtest : subtests)
                        {
                            nanoseconds += subtest->time_measurement.get_mean();
                            test_ok &= subtest->test_ok;
                        }
                        time_measurement.update(nanoseconds);
                        break;
                    }
                    else
                        time_measurement.update(counter.getNanoseconds());
                }
            }
            catch(...)
            {
                stop_counter();
                time_measurement.update(counter.getNanoseconds());
                first_run = false;
                fflush(stdout);
                print_level(term_color::RED);
                color_fputs(term_color::RED, "Exception!\n", DEBUG_OUTPUT_STREAM);
                if(!subtests.empty())
                {
                    fputc('\n', DEBUG_OUTPUT_STREAM);
                    subtests.clear();
                }
                exception_occured = true;
                throw;
            }
        }
        catch(std::exception& e)
        {
            set_term_color(term_color::RED);
            print_level();
            fprintf(DEBUG_OUTPUT_STREAM, "What: %s\n", e.what());
            set_term_color(term_color::DEFAULT);
        }
        catch(...)
        {
            set_term_color(term_color::RED);
            print_level();
            fputs("Not inherited from std::exception!\n", DEBUG_OUTPUT_STREAM);
            set_term_color(term_color::DEFAULT);
        }
        fflush(stdout);
        if(exception_occured)
            test_ok &= exception_expected;
        set_term_color(test_ok ? term_color::GREEN : term_color::RED, DEBUG_OUTPUT_STREAM);
        this->print_test();
        fprintf(DEBUG_OUTPUT_STREAM, " %s ", test_ok ? "succeeded" : "failed!");
        set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
        fprinttimer(DEBUG_OUTPUT_STREAM, time_measurement, counter.getPrecision());
        bool test_ok_ret = run_next() && test_ok;
        test_ok &= test_ok_saved;
        return test_ok_ret;
    }
    
    bool _test_class_abstract::run_next()
    {
        return true;
    }
    
    void _test_class_abstract::print_test(term_color color) const
    {
        set_term_color(color, DEBUG_OUTPUT_STREAM);
        print_level();
        fprintf(DEBUG_OUTPUT_STREAM, "Test: %s", this->test_name);
        if(test_info_str.length())
            fprintf(DEBUG_OUTPUT_STREAM, " <%s>", test_info_str.c_str());
        if(color != term_color::KEEP)
            set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
    }

    void _test_class_abstract::print_level(term_color color) const
    {
        set_term_color(color, DEBUG_OUTPUT_STREAM);
        if(first_run && show_level == g_static::global_static_var<unsigned, "Test"_gsh, "Show level"_gsh>())
            fputc('\n', DEBUG_OUTPUT_STREAM);
        g_static::global_static_var<unsigned, "Test"_gsh, "Show level"_gsh>() = show_level;
        if(show_level < 4)
            for(unsigned i = 0;i < show_level;i++)
                fputc('>', DEBUG_OUTPUT_STREAM);
        else
        {
            char buf[sizeof(show_level) * 3 + 1];
            sprintf(buf, "%u", show_level);
            unsigned len = 0;
            while(buf[++len]);
            len += 2;
            for(unsigned i = 0;i < (show_level < 16 ? show_level : 16) - len;i++)
                fputc('+', DEBUG_OUTPUT_STREAM);
            fprintf(DEBUG_OUTPUT_STREAM, ">%s>", &buf);
        }
        fputc(' ', DEBUG_OUTPUT_STREAM);
        if(color != term_color::KEEP)
            set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
    }
    
    void _test_class_abstract::print_failed() const
    {
        print_test(term_color::RED);
        fputc('\n', DEBUG_OUTPUT_STREAM);
        for(unsigned i = 0;i < subtests.size();i++)
            subtests[i]->print_failed();
        set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
    }
    
    bool _test_class_abstract::error(int line, const char *format, ...)
    {
        if(++errors_occured <= max_error_amount_show)
        {
            set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM);
            print_level();
            va_list args;
            va_start(args, format);
            va_end(args);
            vfprintf(DEBUG_OUTPUT_STREAM, format, args);
            fprintf(DEBUG_OUTPUT_STREAM, " at line %d\n", line);
            if(errors_occured == max_error_amount_show)
            {
                print_level();
                fprintf(DEBUG_OUTPUT_STREAM, "Max amount of errors to show (limit %u)"
                                "\nNo errors will be shown further\n", max_error_amount_show);
            }
        }
        test_ok = false;
        if(errors_occured == errors_to_stop)
        {
            set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM);
            print_level();
            fprintf(DEBUG_OUTPUT_STREAM, "Failed test limit reached(%d)!\n", errors_to_stop);
            print_level();
            fputs("Stopping test execution\n", DEBUG_OUTPUT_STREAM);
            set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
            return false;
        }
        return true;
    }
    
    bool _test_class_abstract::start_counter()
    {
        this->counter_running = 1;
        counter.start();
        return true;
    }
    
    bool _test_class_abstract::stop_counter()
    {
        if(this->counter_running)
            counter.stop();
        return false;
    }
    
    _subtest_class::_subtest_class(_test_class_abstract *parent, const char *subtest_name):
        _test_class_abstract(subtest_name, parent->show_level + 1)
    {
        parent->subtests.push(this);
    }
    
    _subtest_class::_subtest_class(_test_class_abstract *parent, const char *subtest_name, std::function<void(_test_class_abstract*)> init): _subtest_class(parent, subtest_name)
    {
        init(this);
    }
    
    void _subtest_class::test_body()
    {
        this->_test_body(this);
    }
    
    
    _test_pack_class::_test_pack_class(const char *test_pack_name):
            test_pack_name(test_pack_name), errors_to_stop(0),
            test_classes(), test_failed(),
            nanoseconds_all(0), nanoseconds_test(0)
    {}

    _test_pack_class::~_test_pack_class()
    {}

    bool _test_pack_class::test(str mask)
    {
        g_static::global_static_var<unsigned, "Test"_gsh, "Show level"_gsh>() = 0;
        color_fprintf(term_color::BLUE, DEBUG_OUTPUT_STREAM, "Running %s pack test\n", this->test_pack_name);
        test_ok = true;
        nanoseconds_all = 0;
        nanoseconds_test = 0;
        errors_occured = 0;
        bool at_least_one_test_run = false;
        //warming up CPU
        {
            fputs("\nWarming up", DEBUG_OUTPUT_STREAM);
            counter.start();
            volatile unsigned zomg = 1;
            unsigned long long t = 200;
            bool flag = true;
            for(unsigned i = 1;flag;i++)
            {
                zomg *= i++;
                if(zomg >= 100937)
                    zomg /= (i & 7) + 1;
                counter.stop();
                unsigned long long cnt = counter.getMilliseconds();
                counter.cont();
                while(t <= cnt)
                {
                    fputc('.', DEBUG_OUTPUT_STREAM);
                    fflush(DEBUG_OUTPUT_STREAM);
                    if(t >= 500)
                        flag = false;
                    t += 50;
                }
            }
            fputs("\n\n", DEBUG_OUTPUT_STREAM);
        }
        test_failed.clear();
        counter.start();
        for(unsigned i = 0; i < test_classes.size(); i++)
        {
            if(mask == "*" || mask == test_classes[i]->test_name)
            {
                at_least_one_test_run = true;
                bool local_test_ok = test_classes[i]->run();
                test_ok = test_ok && local_test_ok;
                nanoseconds_test += test_classes[i]->time_measurement.get_mean() * test_classes[i]->time_measurement.get_count();
                if(!local_test_ok)
                {
                    test_failed.push(i);
                    if(++errors_occured == errors_to_stop)
                    {
                        color_fprintf(term_color::ORANGE, DEBUG_OUTPUT_STREAM, "Failed test limit reached(%d)!\n",
                                    this->errors_to_stop);
                        fputs("Stopping test pack execution\n", DEBUG_OUTPUT_STREAM);
                        break;
                    }
                }
            }
        }
        counter.stop();
        nanoseconds_all = counter.getNanoseconds();
        if(!at_least_one_test_run)
        {
            color_fprintf(term_color::RED, DEBUG_OUTPUT_STREAM,
                        "No tests in test pack %s covered by mask %s", this->test_pack_name, mask.c_str());
            return false;
        }
        else
            fputc('\n', DEBUG_OUTPUT_STREAM);
        if(test_ok)
            color_fprintf(term_color::GREEN, DEBUG_OUTPUT_STREAM,
                        "Test pack %s: succeeded ", this->test_pack_name)
        else
            color_fprintf(term_color::RED, DEBUG_OUTPUT_STREAM,
                        "Test pack %s: failed! ", this->test_pack_name);
        if((nanoseconds_all - nanoseconds_test) * 20 >= nanoseconds_test)
        {
            fputs("[All: ", DEBUG_OUTPUT_STREAM);
            fprinttimer(DEBUG_OUTPUT_STREAM, welford<VARIANCE>().update(nanoseconds_all), counter.getPrecision(), false);
            fputs(", Tests: ", DEBUG_OUTPUT_STREAM);
            fprinttimer(DEBUG_OUTPUT_STREAM, welford<VARIANCE>().update(nanoseconds_test), counter.getPrecision(), false);
            fputs("]\n", DEBUG_OUTPUT_STREAM);
        }
        else
            fprinttimer(DEBUG_OUTPUT_STREAM, welford<VARIANCE>().update(nanoseconds_test), counter.getPrecision());
        if(!test_ok)
        {
            fputc('\n', DEBUG_OUTPUT_STREAM);
            print_failed();
        }
        fputc('\n', DEBUG_OUTPUT_STREAM);
        return test_ok;
    }

    void _test_pack_class::print_failed() const
    {
        fprintf(DEBUG_OUTPUT_STREAM, "%d test%s failed:\n", this->errors_occured,
                    (this->errors_occured > 1 ? "s" : ""));
        for(unsigned i = 0; i < test_classes.size(); i++)
            if(!test_classes[i]->test_ok)
                test_classes[i]->print_failed();
    }
}
