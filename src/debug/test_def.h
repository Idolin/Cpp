#pragma once

#include "../container/vector.hpp"
#include "../other/global_static.hpp"
#include "tests_abstract.h"
#include "../other/defdef.h"

#include <exception>

//TODO
/*
 * Nested tests
 * Time limit, memory limit, testing n times
 * Expected exception test
 * Summary statistics(How many 0-level test failed/succed, memory peak, which tests failed)
 * Regular expression for test chooser
*/

static vect<_test_abstract_class_::_test_class_abstract *> *_test_classes_main_sequence;

#define COUNT_TIME_IN_MS this -> count_ms_not_clocks = true;
#define COUNT_TIME_IN_CLOCKS this -> count_ms_not_clocks = false;
#define EXCEPTION_EXPECTED this -> exception_expected = true;
#define STOP_AFTER_ERROR(n) this -> errors_to_stop = n;
#define REPEAT(n) this -> test_repeat_amount = n;

#define TEST_PACK(pack_name, ...) \
    struct _test_pack_ ## pack_name: _test_pack_class_abstract \
    { \
        _test_pack_ ## pack_name(): _test_pack_class_abstract((char*)#pack_name) \
        { \
                global_static_var<vect<_test_pack_class_abstract*>, test_global_static_id>() -> push(this); \
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

#define SUBTEST(test_name, ...) \
    puts(test_name);

#define ENDSUBTEST()

#define STOP_AFTER_ERRORS(n)

#define EXPECT_TRUE(a, ...) \
    if(!(a)) \
    { \
        if(++this -> errors_occured <= this -> max_error_amount_show) \
        { \
            set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM); \
            DEBUGLVLMSG_N(5, "> "); \
            DEBUGLVLMSG_N(5, GET_ARG_DEF_("Check(true) failed", ## __VA_ARGS__)); \
            DEBUGLVLMSG(5, " at line %d", __LINE__); \
            set_term_color(term_color::DEFAULT, DEBUG_OUTPUT_STREAM); \
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
