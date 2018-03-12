#include "tests_abstract.h"

static inline void _fputcs(FILE *SOUT, bool s)
{
    if(s)
        fputc('s', SOUT);
    fputc(' ', SOUT);
}

void fprinttime(FILE *SOUT, unsigned long long time, bool is_ms)
{
    fputc('[', SOUT);
    if(is_ms)
    {
        unsigned ms = static_cast<unsigned int>(time % 1000000), s = 0, m = 0, h = 0, d = 0;
        time /= 1000000;
        if(time)
        {
            s = static_cast<unsigned int>(time % 60);
            time /= 60;
        }
        if(time)
        {
            m = static_cast<unsigned int>(time % 60);
            time /= 60;
        }
        if(time)
        {
            h = static_cast<unsigned int>(time % 24);
            time /= 24;
        }
        d = static_cast<unsigned int>(time);
        bool print;
        if((print = (d > 0)))
        {
            fprintf(SOUT, "%u day", d);
            _fputcs(SOUT, (d > 1));
        }
        if((print = print || (h > 0)))
        {
            fprintf(SOUT, "%u hour", h);
            _fputcs(SOUT, (h > 1));
        }
        if((print = print || (m > 0)))
        {
            fprintf(SOUT, "%u minute", m);
            _fputcs(SOUT, (m > 1));
        }
        if((print || (s > 0)))
        {
            fprintf(SOUT, "%u second", s);
            _fputcs(SOUT, (s > 1));
        }
        fprintf(SOUT, "%u microsecond", ms);
        if(ms > 1)
            fputc('s', SOUT);
    } else
        fprintf(SOUT, "%llu clocks", time);
    fputs("]\n", SOUT);
}

namespace _test_abstract_class_
{

    timeval _time;

    _test_class_abstract::_test_class_abstract(char *test_name) : test_name(test_name), test_repeat_amount(1),
                                                                  max_error_amount_show(3), errors_to_stop(0),
                                                                  exception_expected(false)
    {}

    bool _test_class_abstract::run(bool count_ms_not_clocks)
    {
        color_fprintf(term_color::BLUE, DEBUG_OUTPUT_STREAM, "> Test: %s\n", this->test_name);
        exception_occured = false;
        errors_occured = 0;
        test_ok = true;
        if(count_ms_not_clocks)
        {
            gettimeofday(&_time, nullptr);
            test_time_ms_or_clks = static_cast<unsigned long long>(_time.tv_sec * 1000000 +
                                                                   _time.tv_usec); // NOLINT (always casting non-negative value)
        }
        else
            test_time_ms_or_clks = static_cast<unsigned long long>(clock());
        try
        {
            try
            {
                for(unsigned i = 0; i < test_repeat_amount; i++)
                    test_body();
            }
            catch(...)
            {
                fflush(stdout);
                color_fputs(term_color::RED, "> Exception!\n", DEBUG_OUTPUT_STREAM);
                exception_occured = true;
                throw;
            }
        }
        catch(std::exception &e)
        {
            fprintf(DEBUG_OUTPUT_STREAM, "> What:%s\n", e.what());
        }
        catch(...)
        {
            fputs("> Not inherited from std::exception!\n", DEBUG_OUTPUT_STREAM);
        }
        fflush(stdout);
        if(count_ms_not_clocks)
        {
            gettimeofday(&_time, nullptr);
            test_time_ms_or_clks = _time.tv_sec * 1000000 + _time.tv_usec - test_time_ms_or_clks;
        }
        else
            test_time_ms_or_clks = static_cast<unsigned long long>(clock()) - test_time_ms_or_clks;
        if(exception_occured)
            test_ok = test_ok && exception_expected;
        color_fprintf((test_ok ? term_color::GREEN : term_color::RED), DEBUG_OUTPUT_STREAM, "> Test %s ",
                      (test_ok ? "successed" : "failed!"));
        fprinttime(DEBUG_OUTPUT_STREAM, test_time_ms_or_clks, count_ms_not_clocks);
        return test_ok;
    }

};

_test_pack_class_abstract::_test_pack_class_abstract(char *test_pack_name) : test_pack_name(test_pack_name),
                                                                             count_ms_not_clocks(true),
                                                                             errors_to_stop(0),
                                                                             test_classes(
                                                                                     vect<_test_abstract_class_::_test_class_abstract *>()),
                                                                             test_failed(vect<unsigned>())
{}

bool _test_pack_class_abstract::test()
{
    color_fprintf(term_color::BLUE, DEBUG_OUTPUT_STREAM, "Running %s pack test\n\n", this->test_pack_name);
    test_ok = true;
    test_time_ms_or_clks = 0;
    errors_occured = 0;
    for(unsigned i = 0; i < test_classes.maxs; i++)
    {
        bool local_test_ok = test_classes[i]->run(this->count_ms_not_clocks);
        test_ok = test_ok && local_test_ok;
        test_time_ms_or_clks += test_classes[i]->test_time_ms_or_clks;
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
    if(test_ok)
    color_fprintf(term_color::GREEN, DEBUG_OUTPUT_STREAM, "Test pack %s: successed ", this->test_pack_name)
    else
    color_fprintf(term_color::RED, DEBUG_OUTPUT_STREAM, "Test pack %s: failed! ", this->test_pack_name);
    ftimeprint(DEBUG_OUTPUT_STREAM, test_time_ms_or_clks, count_ms_not_clocks);
    if(!test_ok)
    {
        fprintf(DEBUG_OUTPUT_STREAM, "%d test%s failed:\n", this->errors_occured,
                (this->errors_occured > 1 ? "s" : ""));
        for(unsigned i = 0; i < this->errors_occured; i++)
            fprintf(DEBUG_OUTPUT_STREAM, "\t%s\n", test_classes[test_failed[i]]->test_name);
    }
    fputc('\n', DEBUG_OUTPUT_STREAM);
    return test_ok;
}
