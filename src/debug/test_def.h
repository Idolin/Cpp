#pragma once

#include "../container/vector.hpp"
#include "../other/singleton.hpp"
#include "tests_abstract.h"
#include "../other/defdef.h"
#include "def_debug.h"

#include <cmath>
#include <exception>

//TODO
/*
 * Nested tests
 * Time limit, memory limit, testing n times
 * Summary statistics(How many 0-level test failed/succeeded, memory peak, which tests failed)
 * Regular expression for test chooser
*/

static vect<_test_abstract_class_::_test_class_abstract *> *_test_classes_main_sequence;

#define TESTS_ENABLED

#define EXCEPTION_EXPECTED this -> exception_expected = true;
#define STOP_AFTER_ERROR(n) this -> errors_to_stop = n;
#define REPEAT(n) this -> test_repeat_amount = n;

#ifdef TESTS_ENABLED

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

#else

#define TEST_PACK(pack_name, ...) namespace test_namespace_ ## pack_name

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
    \
    void test_ ## test_name::test_body()

#endif // TESTS_ENABLED


#define _EXPECT_TRUE(a, ...) \
    { \
        try \
        { \
            if(!(a)) \
            { \
                if(++this -> errors_occured <= this -> max_error_amount_show) \
                { \
                    set_term_color(term_color::ORANGE, DEBUG_OUTPUT_STREAM); \
                    DEBUGLVLMSG_N(5, "> "); \
                    DEBUGLVLMSG_N(5, ## __VA_ARGS__); \
                    DEBUGLVLMSG(5, " at line %d", __LINE__); \
                    if(this -> print_test_with(term_color::ORANGE)) \
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
                fprintf(DEBUG_OUTPUT_STREAM, ## __VA_ARGS__); \
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
#define EXPECT_TRUE(a, ...) _EXPECT_TRUE(a, GET_ARG_DEF_II("Check(" #a ") failed", ## __VA_ARGS__))
#define EXPECT_FALSE(a, ...) EXPECT_TRUE(!(a), GET_ARG_DEF("Check(!(" #a ")) failed", ## __VA_ARGS__))
#define EXPECT_LT(a, b, ...) EXPECT_TRUE((a) < (b), GET_ARG_DEF("Check((" #a ") < (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_LE(a, b, ...) EXPECT_TRUE((a) <= (b), GET_ARG_DEF("Check((" #a ") <= (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_EQ(a, b, ...) EXPECT_TRUE((a) == (b), GET_ARG_DEF("Check((" #a ") == (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_GE(a, b, ...) EXPECT_TRUE((a) >= (b), GET_ARG_DEF("Check((" #a ") >= (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_GT(a, b, ...) EXPECT_TRUE((a) > (b), GET_ARG_DEF("Check((" #a ") > (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_NE(a, b, ...) EXPECT_TRUE((a) != (b), GET_ARG_DEF("Check((" #a ") != (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_NEAR(a, b, d, ...) \
    { \
        const auto _debug_var_a = (a); \
        const auto _debug_var_b = (b); \
        const auto _debug_var_d = (d); \
        EXPECT_TRUE(((_debug_var_a - _debug_var_b) < _debug_var_d) && ((_debug_var_b - _debug_var_a) < _debug_var_d), GET_ARG_DEF_I("Check((" #a ") - (" #b ") < (" #d ")) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_IN_RANGE(a, l, r, ...) \
    { \
        const auto _debug_var_a = (a); \
        const auto _debug_var_l = (l); \
        const auto _debug_var_r = (r); \
        EXPECT_TRUE((_debug_var_a >= _debug_var_l) && (_debug_var_a <= _debug_var_r), GET_ARG_DEF("Check((" #a " in [(" #l ")..(" #r ")]) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_FLOATING_POINT_EQ(a, b, ...) EXPECT_NEAR(a, b, sqrt(std::numeric_limits<typeof(a)>::epsilon()), GET_ARG_DEF("Check((" #a " ~ " #b ")) failed", ## __VA_ARGS__))
#define EXPECT_FLOAT_EQ(a, b, ...) EXPECT_NEAR(a, b, sqrt(std::numeric_limits<float>::epsilon()), GET_ARG_DEF("Check((" #a " ~ " #b ")) failed", ## __VA_ARGS__))
#define EXPECT_DOUBLE_EQ(a, b, ...) EXPECT_NEAR(a, b, sqrt(std::numeric_limits<double>::epsilon()), GET_ARG_DEF("Check((" #a " ~ " #b ")) failed", ## __VA_ARGS__))
#define EXPECT_CSTRING_EQ(a, b, ...) \
    { \
        unsigned long i = 0; \
        while(a[i] == b[i]) \
            if(a[i] == '\0') \
                break; \
            else \
                i++; \
        EXPECT_TRUE(a[i] == b[i], GET_ARG_DEF("Check strings((" #a ") == (" #b ")) failed", ## __VA_ARGS__)); \
    }
#define _LAST_COMPOSE_EQ(msg, a, b) EXPECT_EQ(a, b, msg + "(" QUOTE(a) "!=" QUOTE(b) ")")
#define COMPOSE_EQ(msg, a, b, ...) EXPECT_EQ(a, b, msg + "(" QUOTE(a) "!=" QUOTE(b) ")"); __VA_ARGS__
#define GET_123(msg, a, b, ...) msg, a, b
#define SKIP2_1(a, b, ...) a, ## __VA_ARGS__
#define EXPECT_EQ_ALL(a, ...) \
    { \
        MULT_ARG_R_N(GET_ARGS_COUNT(__VA_ARGS__), COMPOSE_EQ, GET_123, SKIP2_1, "Check(equals all) failed", a, ## __VA_ARGS__); \
    }
#define EXPECT_EQ_RA(a, b, len, ...) \
    { \
        for(size_t i = 0;i < (len);i++) \
            EXPECT_EQ((a)[i], (b)[i], GET_ARG_DEF("Check((" #a ")[i] == (" #b ")[i], i=0,...,(" #len ")) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_EQ_RA_LEN(a, b, len1, len2, ...) \
    { \
        auto len = (len1); \
        if(len != (len2)) \
            EXPECT_TRUE(false, GET_ARG_DEF("Check((" #len1 ") == (" #len2 ")) failed", ## __VA_ARGS__)); \
        else \
            EXPECT_EQ_RA(a, b, len, GET_ARG_DEF("Check((" #a ")[i] == (" #b ")[i], i=0,...,(" #len1 ") == (" #len2 ")) failed", ## __VA_ARGS__)); \
    }
#define _LAST_COMPOSE_EQ_I(a, i, b) _EXPECT_TRUE((a[(i)]) == (b), "Check(equals random access) failed(" QUOTE(a) "[" QUOTE(i) "] != " QUOTE(b) ")")
#define COMPOSE_EQ_I(a, i, b, ...) _EXPECT_TRUE((a[(i)]) == (b), "Check(equals random access) failed(" QUOTE(a) "[" QUOTE(i) "] != " QUOTE(b) ")"); __VA_ARGS__
#define SKIP_3_1_INC_2(a, i, b, ...) a, NEXT(i), ## __VA_ARGS__
#define EXPECT_EQ_RA_VALS(a, len, ...) \
    { \
        if((len) != GET_ARGS_COUNT(__VA_ARGS__)) \
        { \
            EXPECT_TRUE(false, "Check(random access length) failed((" #len ") == " QUOTE(GET_ARGS_COUNT(__VA_ARGS__)) ") failed"); \
        } \
        else \
        { \
            MULT_ARG_R_N(GET_ARGS_COUNT(__VA_ARGS__), COMPOSE_EQ_I, GET_123, SKIP_3_1_INC_2, a, 0, ## __VA_ARGS__); \
        } \
    }
#define EXPECT_EXCEPTION(a, exception, ...) \
    { \
        this -> test_ok = false; \
        try \
        { \
            a; \
        } \
        catch(const exception&) \
        { \
            this -> test_ok = true; \
        } \
        EXPECT_TRUE(this -> test_ok, GET_ARG_DEF("Check(exception) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_EXCEPTION_ANY(a, ...) \
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
        EXPECT_TRUE(this -> test_ok, GET_ARG_DEF("Check(exception_any) failed", ## __VA_ARGS__)); \
    }

#define COMPOSE_TEST(B, A, X,  ...) \
{ \
    process_time_counter inner; \
    this -> test_with.push(#B " " QUOTE_F(X)); \
    inner.start(); \
    { B X A } \
    inner.stop(); \
    this -> print_level(term_color::CYAN); \
    color_fputs(term_color::CYAN, "> Test ", DEBUG_OUTPUT_STREAM); \
    this -> print_test_with(term_color::CYAN); \
    fputc(' ', DEBUG_OUTPUT_STREAM); \
    fprinttime(DEBUG_OUTPUT_STREAM, inner.getMilliseconds()); \
    this -> test_with.pop(); \
} __VA_ARGS__

#define _LAST_COMPOSE_TEST(...) COMPOSE_TEST(__VA_ARGS__)

#define WITH_ARG_DEF(decl, value) \
    _test_abstract_class_::_test_with_wrapper wrapper_ ## __COUNTER__ ## _ ## __LINE__(this, #decl " = " #value)

// FOR_CLASS(var_name, list to iterate, { body })
// (example: FOR_CLASS(x, 1, foo(3), Class(), { _tshow(x); })
#define FOR_CLASS(class_name, ...) \
{ \
    FOR_EACH_ARG_COMPOSE(COMPOSE_TEST, auto class_name, ## __VA_ARGS__) \
}

// using cycle for to work with brackets
#define SUB_TEST(subtest_name) \
for(bool saved = new_subtest(#subtest_name), _si = false; \
    _si == false; \
    _si = this -> check_subtest(saved))
