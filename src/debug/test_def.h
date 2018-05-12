#pragma once

#include "../container/vector.hpp"
#include "../other/singleton.hpp"
#include "tests_abstract.h"
#include "../other/defdef.h"

#include <exception>

//TODO
/*
 * Nested tests
 * Time limit, memory limit, testing n times
 * Expected exception test
 * Summary statistics(How many 0-level test failed/succeeded, memory peak, which tests failed)
 * Regular expression for test chooser
*/

static vect<_test_abstract_class_::_test_class_abstract *> *_test_classes_main_sequence;

#ifndef DEBUG
    #define DEBUG
#endif

#define EXCEPTION_EXPECTED this -> exception_expected = true;
#define STOP_AFTER_ERROR(n) this -> errors_to_stop = n;
#define REPEAT(n) this -> test_repeat_amount = n;

#define TEST_PACK(pack_name, ...) \
    struct _test_pack_ ## pack_name: _test_pack_class_abstract \
    { \
        _test_pack_ ## pack_name(): _test_pack_class_abstract((char*)#pack_name) \
        { \
                g_static::global_static_var<vect<_test_pack_class_abstract*>, g_static::test_global_static_id>().push(this); \
                __VA_ARGS__; \
                _test_classes_main_sequence = &(this -> test_classes); \
        } \
        ~_test_pack_ ## pack_name() = default; \
    }; \
    \
    _test_pack_ ## pack_name _test_pack_class_ ## pack_name; \
    \
    namespace test_namespace_ ## pack_name

#define TEST(test_name, ...) \
    struct test_ ## test_name: _test_abstract_class_::_test_class_abstract \
    { \
        test_ ## test_name(): _test_class_abstract((char*)#test_name) \
        { \
            _test_classes_main_sequence -> push(this); \
            __VA_ARGS__; \
        } \
        ~test_ ## test_name() = default; \
        void test_body(); \
    }; \
    test_ ## test_name test_ ## test_name_single_class_ ## test_name; \
    \
    void test_ ## test_name::test_body()

#define EXPECT_TRUE(a, ...) \
    { \
        try \
        { \
            if(!(a)) \
            { \
                if(++this -> errors_occured <= this -> max_error_amount_show) \
                { \
                    set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM); \
                    DEBUGLVLMSG_N(5, "> "); \
                    DEBUGLVLMSG_N(5, GET_ARG_DEF_("Check(true) failed", ## __VA_ARGS__)); \
                    DEBUGLVLMSG(5, " at line %d", __LINE__); \
                    this -> print_test_with(term_color::ORANGE); \
                    fputc('\n', DEBUG_OUTPUT_STREAM); \
                    if(this -> errors_occured == this -> max_error_amount_show) \
                        DEBUGLVLMSG(4, "> Max amount of errors to show (limit %u)" \
                                       "\nNo errors will be shown further", \
                                        this -> max_error_amount_show); \
                } \
                this -> test_ok = false; \
                if(this -> errors_occured == this -> errors_to_stop) \
                { \
                    set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM); \
                    DEBUGLVLMSG(4, "> Failed test limit reached(%d)!", this -> errors_to_stop); \
                    set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM); \
                    DEBUGLVLMSG(4, "> Stopping test execution"); \
                    return; \
                } \
            } \
        } \
        catch(...) \
        { \
            this -> test_ok = false; \
            if(++this -> errors_occured <= this -> max_error_amount_show) \
            { \
                set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM); \
                this -> print_level(); \
                fprintf(DEBUG_OUTPUT_STREAM, "> "); \
                fprintf(DEBUG_OUTPUT_STREAM, GET_ARG_DEF_("Check(true) failed", ## __VA_ARGS__)); \
                fprintf(DEBUG_OUTPUT_STREAM, " at line %d\n", __LINE__); \
                this -> print_test_with(term_color::ORANGE); \
                fputc('\n', DEBUG_OUTPUT_STREAM); \
            } \
            this -> test_ok = false; \
            if(this -> errors_occured == this -> errors_to_stop) \
            { \
                set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM); \
                this -> print_level(); \
                fprintf(DEBUG_OUTPUT_STREAM, "> Failed test limit reached(%d)!\n", this -> errors_to_stop); \
                fprintf(DEBUG_OUTPUT_STREAM, "> Stopping test execution\n"); \
                set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM); \
                return; \
            } \
            throw; \
        } \
    }
#define EXPECT_FALSE(a, ...) EXPECT_TRUE(!(a), GET_ARG_DEF("Check(false) failed", ## __VA_ARGS__))
#define EXPECT_LT(a, b, ...) EXPECT_TRUE((a) < (b), GET_ARG_DEF("Check(lower) failed", ## __VA_ARGS__))
#define EXPECT_LE(a, b, ...) EXPECT_TRUE((a) <= (b), GET_ARG_DEF("Check(lower ot equals) failed", ## __VA_ARGS__))
#define EXPECT_EQ(a, b, ...) EXPECT_TRUE((a) == (b), GET_ARG_DEF("Check(equals) failed", ## __VA_ARGS__))
#define EXPECT_GE(a, b, ...) EXPECT_TRUE((a) >= (b), GET_ARG_DEF("Check(greater or equals) failed", ## __VA_ARGS__))
#define EXPECT_GT(a, b, ...) EXPECT_TRUE((a) > (b), GET_ARG_DEF("Check(greater) failed", ## __VA_ARGS__))
#define EXPECT_NE(a, b, ...) EXPECT_TRUE((a) != (b), GET_ARG_DEF("Check(not equals) failed", ## __VA_ARGS__))
#define EXPECT_NEAR(a, b, d, ...) EXPECT_TRUE((((a) - (b)) < (d)) && (((b) - (a)) < (d)), GET_ARG_DEF("Check(not near) failed", ## __VA_ARGS__))
#define EXPECT_DOUBLE_EQ(a, b, ...) EXPECT_NEAR(a, b, 0.000000000000001L, GET_ARG_DEF("Check(equals double) failed", ## __VA_ARGS__))
#define EXPECT_STRING_EQ(a, b, ...) \
    { \
        unsigned long i = 0; \
        while(a[i] == b[i]) \
            if(a[i] == '\0') \
                break; \
            else \
                i++; \
        EXPECT_TRUE(a[i] == b[i], GET_ARG_DEF("Check(string equals) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_EXCEPTION(a, ...) \
    { \
        this -> test_ok = false; \
        try \
        { \
            a; \
        } \
        catch(...) \
        { \
            this -> test_ok = true; \
        } \
        EXPECT_TRUE(this -> test_ok, GET_ARG_DEF("Check(exception) failed", ## __VA_ARGS__)); \
    }

#define COMPOSE_TEST(B, X, A) \
{ \
    process_time_counter inner; \
    this -> test_with.push(#B " " #X); \
    inner.start(); \
    { B X A } \
    inner.stop(); \
    this -> print_level(term_color::CYAN); \
    color_fputs(term_color::CYAN, "> Test ", DEBUG_OUTPUT_STREAM); \
    this -> print_test_with(term_color::CYAN); \
    fputc(' ', DEBUG_OUTPUT_STREAM); \
    fprinttime(DEBUG_OUTPUT_STREAM, inner.getMilliseconds()); \
    this -> test_with.pop(); \
}

// FOR_CLASS(var_name, list to iterate, { body })
// (example: FOR_CLASS(x, 1, foo(3), Class(), { _tshow(x); })
#define FOR_CLASS(class_name, ...) \
{ \
    FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, auto class_name, ## __VA_ARGS__) \
}

#define SUB_TEST(subtest_name) \
for(bool saved = new_subtest(#subtest_name), _si = false; \
    _si == false; \
    _si = this -> check_subtest(saved))
