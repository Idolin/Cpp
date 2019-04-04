#include "tests_abstract.h"

void fprinttime(FILE *SOUT, unsigned long long milliseconds, const char *add_str)
{
    const char* identifier[5] = {"day", "hour", "minute", "second", "millisecond"};
    const unsigned coefficient[4] = {24, 60, 60, 1000};

    fputc('[', SOUT);
    unsigned value[5];
    unsigned start_from = 5;
    while(--start_from > 0)
    {
        value[start_from] = static_cast<unsigned>(milliseconds % coefficient[start_from - 1]);
        if((milliseconds /= coefficient[start_from - 1]) == 0)
            break;
    }
    value[0] = static_cast<unsigned>(milliseconds);
    for(unsigned i = start_from;i < 5;i++)
    {
        fprintf(SOUT, "%u %s", value[i], identifier[i]);
        if(value[i] != 1)
            fputc('s', SOUT);
        if(i < 4)
            fputc(' ', SOUT);
    }
    if(add_str)
    {
        fputc(' ', SOUT);
        fputs(add_str, SOUT);
    }
    fputs("]\n", SOUT);
}

namespace _test_abstract_class_
{
    _test_class_abstract::_test_class_abstract(const char *test_name):
            test_name(test_name), test_repeat_amount(1),
            max_error_amount_show(30), errors_to_stop(0),
            exception_expected(false), test_with(), subtests()
    {}

    bool _test_class_abstract::run()
    {
        color_fprintf(term_color::BLUE, DEBUG_OUTPUT_STREAM, "> Test: %s\n", this->test_name);
        exception_occured = false;
        errors_occured = 0;
        test_ok = true;
        subtests_failed.clear();
        counter.start();
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
                for(unsigned i = 0;i < subtests.size();i++)
                    fprintf(DEBUG_OUTPUT_STREAM, "%s subtest %s",
                            i == 0 ? "In" : ", in", subtests[i].first);
                if(!subtests.empty())
                {
                    fputc('\n', DEBUG_OUTPUT_STREAM);
                    subtests.clear();
                }
                print_test_with();
                exception_occured = true;
                throw;
            }
        }
        catch(std::exception& e)
        {
            fprintf(DEBUG_OUTPUT_STREAM, "> What: %s\n", e.what());
        }
        catch(...)
        {
            fputs("> Not inherited from std::exception!\n", DEBUG_OUTPUT_STREAM);
        }
        fflush(stdout);
        counter.stop();
        if(exception_occured)
            test_ok &= exception_expected;
        color_fprintf((test_ok ? term_color::GREEN : term_color::RED), DEBUG_OUTPUT_STREAM, "> Test %s ",
                      (test_ok ? "succeeded" : "failed!"));
        fprinttime(DEBUG_OUTPUT_STREAM, counter.getMilliseconds());
        if(test_ok && (test_repeat_amount > 1))
        {
            putchar(' ');
            fprinttime(DEBUG_OUTPUT_STREAM, counter.getMilliseconds() / test_repeat_amount, "per run");
        }
        fputc('\n', DEBUG_OUTPUT_STREAM);
        return test_ok;
    }

    void _test_class_abstract::print_test_with(term_color color)
    {
        set_term_color(color, DEBUG_OUTPUT_STREAM);
        for(unsigned i = 0;i < this -> test_with.size();i++)
            if(i + 1 == this -> test_with.size())
                DEBUGLVLMSG_N(5, "with %s", this -> test_with[i])
            else
                DEBUGLVLMSG_N(5, "with %s, ", this -> test_with[i]);
        set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
    }

    void _test_class_abstract::print_level(term_color color)
    {
        set_term_color(color, DEBUG_OUTPUT_STREAM);
        for(unsigned i = 0;i <= subtests.size() + (test_with.empty() ? 0 : 1);i++)
            fputc('>', DEBUG_OUTPUT_STREAM);
        if(color != term_color::KEEP)
            set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
    }

    bool _test_class_abstract::new_subtest(const char *subtest_name)
    {
        subtests.push(std::make_pair(subtest_name, process_time_counter()));
        subtests_failed.push(subtest_name);
        set_term_color(term_color::MAGENTA, DEBUG_OUTPUT_STREAM);
        fputc('\n', DEBUG_OUTPUT_STREAM);
        print_level();
        fprintf(DEBUG_OUTPUT_STREAM, " Subtest: %s\n", subtests.back().first);
        bool saved = this -> test_ok;
        test_ok = true;
        subtests.back().second.start();
        return saved;
    }

    bool _test_class_abstract::check_subtest(bool saved)
    {
        subtests.back().second.stop();
        unsigned long long time = subtests.back().second.getMilliseconds();
        if(!this -> test_ok)
        {
            set_term_color(term_color::RED, DEBUG_OUTPUT_STREAM);
            subtests_failed.push(nullptr);
        }
        else
        {
            set_term_color(term_color::MAGENTA, DEBUG_OUTPUT_STREAM);
            subtests_failed.pop();
        }
        print_level();
        fprintf(DEBUG_OUTPUT_STREAM, " Subtest %s: %s ", subtests.pop().first,
                this -> test_ok ? "succeeded" : "failed");
        fprinttime(DEBUG_OUTPUT_STREAM, time);
        fputc('\n', DEBUG_OUTPUT_STREAM);
        set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM);
        this -> test_ok &= saved;
        return true;
    }

    _test_with_wrapper::_test_with_wrapper(_test_class_abstract *p, const char *with):
        test_p(p)
    {
        test_p -> test_with.push(with);
    }

    _test_with_wrapper::~_test_with_wrapper()
    {
        test_p -> test_with.pop();
    }
};

_test_pack_class_abstract::_test_pack_class_abstract(const char *test_pack_name):
        test_pack_name(test_pack_name), errors_to_stop(0),
        test_classes(), test_failed(),
        milliseconds(0)
{}

bool _test_pack_class_abstract::test(str mask)
{
    color_fprintf(term_color::BLUE, DEBUG_OUTPUT_STREAM, "Running %s pack test\n\n", this->test_pack_name);
    test_ok = true;
    milliseconds = 0;
    errors_occured = 0;
    bool at_least_one_test_run = false;
    test_failed.clear();
    for(unsigned i = 0; i < test_classes.size(); i++)
    {
        if(mask == "*" || mask == test_classes[i]->test_name)
        {
            at_least_one_test_run = true;
            bool local_test_ok = test_classes[i]->run();
            test_ok = test_ok && local_test_ok;
            milliseconds += test_classes[i]->counter.getMilliseconds();
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
    if(!at_least_one_test_run)
    {
        color_fprintf(term_color::RED, DEBUG_OUTPUT_STREAM,
                      "No tests in test pack %s covered by mask %s", this->test_pack_name, mask.c_str());
        return false;
    }
    if(test_ok)
        color_fprintf(term_color::GREEN, DEBUG_OUTPUT_STREAM,
                      "Test pack %s: succeeded ", this->test_pack_name)
    else
        color_fprintf(term_color::RED, DEBUG_OUTPUT_STREAM,
                      "Test pack %s: failed! ", this->test_pack_name);
    fprinttime(DEBUG_OUTPUT_STREAM, milliseconds);
    if(!test_ok)
    {
        fprintf(DEBUG_OUTPUT_STREAM, "%d test%s failed:\n", this->errors_occured,
                (this->errors_occured > 1 ? "s" : ""));
        for(unsigned i = 0; i < this->errors_occured; i++)
        {
            fprintf(DEBUG_OUTPUT_STREAM, "\t%s", test_classes[test_failed[i]]->test_name);
            if(test_classes[test_failed[i]]->subtests_failed.size() == 0)
                fputc('\n', DEBUG_OUTPUT_STREAM);
            else
            {
                fputs(" with failed subtests:\n", DEBUG_OUTPUT_STREAM);
                unsigned level = 1;
                for(const char *s : test_classes[test_failed[i]]->subtests_failed)
                    if(s)
                    {
                        level++;
                        for(unsigned j = 0;j < level;j++)
                            fputc('\t', DEBUG_OUTPUT_STREAM);
                        fprintf(DEBUG_OUTPUT_STREAM, "%s\n", s);
                    }
                    else
                        level--;
            }
        }
    }
    fputc('\n', DEBUG_OUTPUT_STREAM);
    return test_ok;
}
