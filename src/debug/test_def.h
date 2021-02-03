#pragma once

#include "../container/vector.hpp"
#include "../other/singleton.hpp"
#include "tests_abstract.h"
#include "../other/defdef.h"
#include "def_debug.h"
#include "../string/cstr.h"
#include "../template/t_useful.hpp"

#include <cmath>
#include <exception>
#include <iterator>

//TODO
/*
 * Nested tests
 * Time limit, memory limit
 * Summary statistics(memory peak, which tests failed)
 * Regular expression for test chooser
*/

static vect<test_namespace::_test_class_abstract *> *test_classes_main_sequence;

namespace test_namespace
{
    struct comma_split_iterator: public std::iterator<std::input_iterator_tag, cstr>
    {
        private:
            const char *s;
            mutable unsigned next;

        public:
            comma_split_iterator(const char *s);

            comma_split_iterator(const comma_split_iterator& otr);

            comma_split_iterator(comma_split_iterator&& otr);

            comma_split_iterator& operator=(const comma_split_iterator& otr);

            comma_split_iterator& operator=(comma_split_iterator&& otr);

            bool operator==(const comma_split_iterator& otr) const;
            
            bool operator!=(const comma_split_iterator& otr) const;

            comma_split_iterator& operator++();

            comma_split_iterator operator++(int);

            cstr operator*() const;
            
            operator bool() const;
            
        private:
            void get_next() const;
    };
}

#define TESTS_ENABLED

#define EXCEPTION_EXPECTED _this_test_ptr -> exception_expected = true;
#define STOP_AFTER_ERROR(n) _this_test_ptr -> errors_to_stop = n;
#define REPEAT(n) _this_test_ptr -> test_repeat_amount = n;
#define TEST_INFO_STR(s) _this_test_ptr -> test_info_str = s;

#define GET_TEST_TDEF(i, t_name, ...) \
    typedef typename TSeq::template type_n<i>::type t_name;
#define SKIP2_1(a, b, ...) a, ## __VA_ARGS__
#define SKIP_2_1_INC_1(i, a, ...) NEXT(i), a, ## __VA_ARGS__
#define SKIP_3_1_INC_1(i, a, b, ...) NEXT(i), a, ## __VA_ARGS__

#ifdef TESTS_ENABLED

#define TEST_COMPOSE_TYPE_GET(...) __VA_ARGS__
    
#define LAST_TEST_COMPOSE_TYPE_GET(...) 

#define TEST_PACK(pack_name, ...) \
    struct _test_pack_ ## pack_name: test_namespace::_test_pack_class \
    { \
        _test_pack_ ## pack_name(): test_namespace::_test_pack_class(#pack_name) \
        { \
                g_static::global_static_var<vect<test_namespace::_test_pack_class*>, "Test"_gsh, "All test packs"_gsh>().push(this); \
                auto _this_test_ptr = this; \
                __VA_ARGS__; \
                test_classes_main_sequence = &(this -> test_classes); \
        } \
        ~_test_pack_ ## pack_name() = default; \
    }; \
    \
    _test_pack_ ## pack_name _test_pack_class_ ## pack_name; \
    \
    namespace test_namespace_ ## pack_name

#define TEST_WITH_TYPES(test_name, types, ...) \
    \
    template<typename TypeSeqAdapter> \
    struct test_ ## test_name; \
    \
    template<typename TypeSeqAdapter> \
    test_namespace::_test_class_abstract* _get_next_test_ ## test_name(test_namespace::_test_class_abstract* outer) \
    { \
        return new test_ ## test_name <TypeSeqAdapter>(outer); \
    } \
    \
    template<> \
    test_namespace::_test_class_abstract* _get_next_test_ ## test_name<void>(test_namespace::_test_class_abstract* outer) \
    { \
        return nullptr; \
    } \
    \
    template<typename TypeSeqAdapter> \
    struct test_ ## test_name: test_namespace::_test_class_abstract \
    { \
        typedef typename TypeSeqAdapter::TypeSeq TSeq; \
        MULT_ARG_R_N(CALL_I(GET_ARGS_COUNT, types), TEST_COMPOSE_TYPE_GET, GET_TEST_TDEF, SKIP_2_1_INC_1, 0, CALL_I(SKIP_1 , types)) \
        \
        test_namespace::_test_class_abstract* outer; \
        \
        test_ ## test_name(test_namespace::_test_class_abstract* outer=nullptr): \
            _test_class_abstract(#test_name, 2), outer(outer) \
        { \
            outer->subtests.push(this); \
            __VA_ARGS__; \
        } \
        \
        ~test_ ## test_name() = default; \
        \
        void test_body() override; \
    }; \
    \
    template<typename TypeSeqAdapter> \
    struct testouter_ ## test_name: test_namespace::_test_class_abstract \
    { \
        testouter_ ## test_name(): _test_class_abstract(#test_name) \
        { \
            test_classes_main_sequence -> push(this); \
            __VA_ARGS__; \
        } \
        \
        ~testouter_ ## test_name() = default; \
        \
        void test_body() override \
        { \
            test_namespace::_test_class_abstract* next = _get_next_test_ ## test_name <TypeSeqAdapter>(this); \
            if(next == nullptr) \
                return; \
            next->run(); \
        } \
        \
        template<typename Adapter> \
        void run_with_adapter() \
        { \
            test_namespace::_test_class_abstract* next = _get_next_test_ ## test_name <Adapter>(this); \
            if(next == nullptr) \
                return; \
            next->run(); \
            run_with_adapter<typename Adapter::NextAdapter>(); \
        } \
    }; \
    \
    testouter_ ## test_name<CALL(FREE, CALL_I(GET_FIRST, types))> test_ ## test_name_single_class_ ## test_name; \
    \
    template<typename TypeSeqAdapter> \
    void test_ ## test_name <TypeSeqAdapter>::test_body()

#define TEST(test_name, ...) \
    struct test_ ## test_name: test_namespace::_test_class_abstract \
    { \
        test_ ## test_name(): _test_class_abstract(#test_name) \
        { \
            test_classes_main_sequence -> push(this); \
            __VA_ARGS__; \
        } \
        ~test_ ## test_name() = default; \
        void test_body(); \
    }; \
    test_ ## test_name test_ ## test_name_single_class_ ## test_name; \
    \
    void test_ ## test_name::test_body()
    
#define SUBTEST_CSTR_NAME(subtest_name, ...) \
    for(test_namespace::_subtest_class *_subtest_ = \
            new test_namespace::_subtest_class(_this_test_ptr, subtest_name, \
                [&](test_namespace::_test_class_abstract *_this_test_ptr) \
                    { REPEAT(_this_test_ptr->test_repeat_amount);  __VA_ARGS__} \
                );_subtest_->first_run;) \
        for(unsigned char _test_subtest_init = 0; _test_subtest_init < 2; _test_subtest_init++) \
            if(_test_subtest_init) \
                _subtest_ -> run(); \
            else \
                _subtest_ -> _test_body = [&](test_namespace::_test_class_abstract *_this_test_ptr)
                
#define SUBTEST(subtest_name, ...) SUBTEST_CSTR_NAME(#subtest_name, ## __VA_ARGS__)
    
#define TIMER_START \
    { \
        _this_test_ptr->counter_running = 2; \
        _this_test_ptr->counter.start(); \
    }
#define TIMER_CONTINUE \
    { \
        _this_test_ptr->counter_running = 2; \
        _this_test_ptr->counter.cont(); \
    }
#define TIMER_STOP \
    { \
        _this_test_ptr->counter_running = 0; \
        _this_test_ptr->counter.stop(); \
    }

#define WITH_VALUES(_value_type_, _value_name_, ...) \
    for(test_namespace::comma_split_iterator _test_iterate_value = test_namespace::comma_split_iterator(QUOTE(__VA_ARGS__)); _test_iterate_value;) \
        for(_value_type_ _value_name_ : std::initializer_list<_value_type_>{  __VA_ARGS__ }) \
            SUBTEST_CSTR_NAME(_this_test_ptr->test_name, TEST_INFO_STR(cstr("with " #_value_type_ " " #_value_name_ " = ") + *_test_iterate_value++))

#define _EXPECT_TRUE(a, ...) \
    { \
        try \
        { \
            if(!(a)) \
            { \
               if(! _this_test_ptr -> error(__LINE__, ## __VA_ARGS__)) \
                   return; \
            } \
        } \
        catch(...) \
        { \
            if(! _this_test_ptr -> error(__LINE__, ## __VA_ARGS__)) \
                return; \
            throw; \
        } \
    }
            
#endif // TESTS_ENABLED

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
#define LAST_COMPOSE_EQ(msg, a, b) EXPECT_EQ(a, b, msg + "(" QUOTE(a) "!=" QUOTE(b) ")")
#define COMPOSE_EQ(msg, a, b, ...) EXPECT_EQ(a, b, msg + "(" QUOTE(a) "!=" QUOTE(b) ")"); __VA_ARGS__

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
#define LAST_COMPOSE_EQ_I(i, a, b) _EXPECT_TRUE((a[(i)]) == (b), "Check(equals random access) failed(" QUOTE(a) "[" QUOTE(i) "] != " QUOTE(b) ")")
#define COMPOSE_EQ_I(i, a, b, ...) _EXPECT_TRUE((a[(i)]) == (b), "Check(equals random access) failed(" QUOTE(a) "[" QUOTE(i) "] != " QUOTE(b) ")"); __VA_ARGS__
#define EXPECT_EQ_RA_VALS(a, len, ...) \
    { \
        if((len) != GET_ARGS_COUNT(__VA_ARGS__)) \
        { \
            EXPECT_TRUE(false, "Check(random access length) failed((" #len ") == " QUOTE(GET_ARGS_COUNT(__VA_ARGS__)) ") failed"); \
        } \
        else \
        { \
            MULT_ARG_R_N(GET_ARGS_COUNT(__VA_ARGS__), COMPOSE_EQ_I, GET_3, SKIP_3_1_INC_1, 0, a, ## __VA_ARGS__); \
        } \
    }
#define EXPECT_EXCEPTION(a, exception, ...) \
    { \
        bool _test_exception_check = false; \
        try \
        { \
            a; \
        } \
        catch(const exception&) \
        { \
            _test_exception_check = true; \
        } \
        EXPECT_TRUE(_test_exception_check, GET_ARG_DEF("Check(exception) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_EXCEPTION_ANY(a, ...) \
    { \
        bool _test_exception_check = false; \
        try \
        { \
            a; \
        } \
        catch(...) \
        { \
            _test_exception_check = true; \
        } \
        EXPECT_TRUE(_test_exception_check, GET_ARG_DEF("Check(exception_any) failed", ## __VA_ARGS__)); \
    }
