#pragma once

#include "../container/vector.hpp"
#include "../other/singleton.hpp"
#include "tests_abstract.h"
#include "../other/defdef.h"
#include "def_debug.h"
#include "../string/cstr.h"

#include "../template/t_useful.hpp"
#include "../template/compile_time.hpp"
#include "./type_generator.hpp"

#include <cmath>
#include <exception>
#include <iterator>


//TODO
/*
 * Nested tests
 * Time limit, memory limit
 * Memory usage statistics
 * Regular expression for test chooser
*/

extern vect<test_namespace::_test_class_abstract*> *test_classes_main_sequence;

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

#define EXCEPTION_EXPECTED _this_test_ptr -> exception_expected = true
#define STOP_AFTER_ERROR(...) _this_test_ptr -> errors_to_stop = __VA_ARGS__
#define REPEAT(...) _this_test_ptr -> test_repeat_amount = __VA_ARGS__
#define TEST_INFO_STR(...) _this_test_ptr -> test_info_str = __VA_ARGS__

#define GET_TEST_TDEF(i, t_name, ...) \
    typedef typename TSeq::template type_n<i>::type t_name;
#define SKIP2_1(a, b, ...) a, ## __VA_ARGS__
#define SKIP_3_1_INC_1(i, a, b, ...) NEXT_NUM(i), a, ## __VA_ARGS__

#ifdef TESTS_ENABLED

#define TEST_COMPOSE_TYPE_GET(...) __VA_ARGS__
    
#define LAST_TEST_COMPOSE_TYPE_GET(...) 

// TEST_PACK(test_pack_name[, test_pack_options])
#define TEST_PACK(...) \
    namespace \
    { \
        struct CONCAT(_test_pack_, GET_FIRST(__VA_ARGS__)): test_namespace::_test_pack_class \
        { \
            CONCAT(_test_pack_, GET_FIRST(__VA_ARGS__))(): test_namespace::_test_pack_class(QUOTE_W(GET_FIRST(__VA_ARGS__))) \
            { \
                    g_static::global_static_var<vect<test_namespace::_test_pack_class*>, "Test"_gsh, "All test packs"_gsh>().push(this); \
                    auto _this_test_ptr = this; \
                    MAYBE_UNUSED(_this_test_ptr); \
                    SKIP_1L(__VA_ARGS__); \
                    test_classes_main_sequence = &(this->test_classes); \
            } \
            ~CONCAT(_test_pack_, GET_FIRST(__VA_ARGS__))() = default; \
        }; \
        \
        CONCAT(_test_pack_, GET_FIRST(__VA_ARGS__)) CONCAT(_test_pack_class_, GET_FIRST(__VA_ARGS__)); \
    } \
    \
    namespace CONCAT(test_namespace_, GET_FIRST(__VA_ARGS__))

// TEST(test_name[, test_options...])
#define TEST(...) \
    struct CONCAT(_test_, GET_FIRST(__VA_ARGS__)): test_namespace::_test_class_abstract \
    { \
        CONCAT(_test_, GET_FIRST(__VA_ARGS__))(): _test_class_abstract(QUOTE_W(GET_FIRST(__VA_ARGS__))) \
        { \
            test_classes_main_sequence->push(this); \
            SKIP_1L(__VA_ARGS__); \
        } \
        ~CONCAT(_test_, GET_FIRST(__VA_ARGS__))() = default; \
        void test_body(); \
    }; \
    CONCAT(_test_, GET_FIRST(__VA_ARGS__)) CONCAT(_test_test_name_single_class_, GET_FIRST(__VA_ARGS__)); \
    \
    void CONCAT(_test_, GET_FIRST(__VA_ARGS__))::test_body()

#define GET_TYPENAMES_(...) FREE_L(GET_FIRST(__VA_ARGS__))
#define GET_TYPENAMES(...) CALL_W(GET_TYPENAMES_, GET_FIRST(__VA_ARGS__))
// GCC BUG: _testwts_ ## test_name::test_with_types<void> and _testwts_ ## test_name::test_body_type_recursive<void>
//  without Dummy typename throws compile error: explicit specialization in non-namespace scope (tested on gcc version 12.1.0)
// see bug report: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=85282
// TEST_WITH_TYPES(test_name, ((type_names...), type_seq_generator)[, test_options...])
#define TEST_WITH_TYPES(test_name, ...) \
    template<typename... TDesc> \
    struct _testwt_ ## test_name: test_namespace::_test_class_abstract \
    { \
        _testwt_ ## test_name(): test_namespace::_test_class_abstract(#test_name, 2) \
        { \
            TEST_INFO_STR(cstr("with " QUOTE_W(GET_TYPENAMES(__VA_ARGS__)) " = ") + StrDefined<Join<CommaDelimiter, type_generator::TypeDescriptionStr<TDesc>...>>::value); \
            SKIP_1L(SKIP_1E(__VA_ARGS__)); \
        } \
        \
        template<GET_TYPENAMES(__VA_ARGS__)> \
        void test_body_impl(); \
        \
        void test_body() override \
        { \
            test_body_impl<typename TDesc::type...>(); \
        } \
    }; \
    \
    template<typename TypeGenerator> \
    struct _testwts_ ## test_name: test_namespace::_test_class_abstract \
    { \
        _testwts_ ## test_name(): test_namespace::_test_class_abstract(#test_name) \
        { \
            test_classes_main_sequence -> push(this); \
            SKIP_1L(SKIP_1E(__VA_ARGS__)); \
        } \
        \
        template<typename TypeDescTuple, typename Dummy = void> \
        struct test_with_types \
        { \
            static void run() \
            { \
                TypeDescTuple::template instantiate<_testwt_ ## test_name>().run(); \
            } \
        }; \
        \
        template<typename Dummy> \
        struct test_with_types<void, Dummy> \
        { \
            static void run() \
            {} \
        }; \
        \
        template<typename TypeGeneratorRecursive, typename Dummy = void> \
        struct test_body_type_recursive \
        { \
            static void run() \
            { \
                test_with_types<typename TypeGeneratorRecursive::CurrentTypes>::run(); \
                test_body_type_recursive<typename TypeGeneratorRecursive::Next>::run(); \
            } \
        }; \
        \
        template<typename Dummy> \
        struct test_body_type_recursive<void, Dummy> \
        { \
            static void run() \
            {} \
        }; \
        \
        void test_body() override \
        { \
            test_body_type_recursive<TypeGenerator>::run(); \
        } \
    }; \
    \
    _testwts_ ## test_name<CALL(SKIP_1E, GET_FIRST(__VA_ARGS__))> _test_test_name_single_class_ ## test_name; \
    \
    template<typename... TDesc> \
    template<GET_TYPENAMES(__VA_ARGS__)> \
    void _testwt_ ## test_name<TDesc...>::test_body_impl() \

#define SUBTEST_CSTR_NAME(subtest_name, ...) \
    for(test_namespace::_subtest_class *_subtest_ = \
            new test_namespace::_subtest_class(_this_test_ptr, subtest_name, \
                [&](test_namespace::_test_class_abstract *_this_test_ptr) \
                    { REPEAT(_this_test_ptr->test_repeat_amount);  __VA_ARGS__; } \
                );_subtest_->first_run;) \
        for(unsigned char _test_subtest_init = 0; _test_subtest_init < 2; _test_subtest_init++) \
            if(_test_subtest_init) \
                _subtest_ -> run(); \
            else \
                _subtest_ -> _test_body = [&](test_namespace::_test_class_abstract *_this_test_ptr)
                
#define SUBTEST(subtest_name, ...) SUBTEST_CSTR_NAME(#subtest_name, __VA_ARGS__)
    
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
        _this_test_ptr->counter.stop(); \
        _this_test_ptr->counter_running = 0; \
    }

#define WITH_VALUES(_value_type_, _value_name_, ...) \
    for(test_namespace::comma_split_iterator _test_iterate_value = test_namespace::comma_split_iterator(QUOTE(__VA_ARGS__)); _test_iterate_value;) \
        for(_value_type_ _value_name_ : std::initializer_list<_value_type_>{  __VA_ARGS__ }) \
            SUBTEST_CSTR_NAME(_this_test_ptr->test_name, TEST_INFO_STR(cstr("with " #_value_type_ " " #_value_name_ " = ") + *_test_iterate_value++))

#define TEST_INTR_EXPECT_TRUE(a, ...) \
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

#define EXPECT_TRUE(a, ...) TEST_INTR_EXPECT_TRUE(a, GET_ARG_DEF2_M("%s", "Check(" #a ") failed", ## __VA_ARGS__))
#define EXPECT_FALSE(a, ...) TEST_INTR_EXPECT_TRUE(!(a), GET_ARG_DEF2_M("%s", "Check(!(" #a ")) failed", ## __VA_ARGS__))
#define EXPECT_LT(a, b, ...) TEST_INTR_EXPECT_TRUE((a) < (b), GET_ARG_DEF2_M("%s", "Check((" #a ") < (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_LE(a, b, ...) TEST_INTR_EXPECT_TRUE((a) <= (b), GET_ARG_DEF2_M("%s", "Check((" #a ") <= (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_EQ(a, b, ...) TEST_INTR_EXPECT_TRUE((a) == (b), GET_ARG_DEF2_M("%s", "Check((" #a ") == (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_GE(a, b, ...) TEST_INTR_EXPECT_TRUE((a) >= (b), GET_ARG_DEF2_M("%s", "Check((" #a ") >= (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_GT(a, b, ...) TEST_INTR_EXPECT_TRUE((a) > (b), GET_ARG_DEF2_M("%s", "Check((" #a ") > (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_NE(a, b, ...) TEST_INTR_EXPECT_TRUE((a) != (b), GET_ARG_DEF2_M("%s", "Check((" #a ") != (" #b ")) failed", ## __VA_ARGS__))
#define EXPECT_NEAR(a, b, d, ...) \
    { \
        const auto _debug_var_a = (a); \
        const auto _debug_var_b = (b); \
        const auto _debug_var_d = (d); \
        TEST_INTR_EXPECT_TRUE(((_debug_var_a - _debug_var_b) < _debug_var_d) && ((_debug_var_b - _debug_var_a) < _debug_var_d), GET_ARG_DEF2_M("%s", "Check((" #a ") - (" #b ") < (" #d ")) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_IN_RANGE(a, l, r, ...) \
    { \
        const auto _debug_var_a = (a); \
        const auto _debug_var_l = (l); \
        const auto _debug_var_r = (r); \
        TEST_INTR_EXPECT_TRUE((_debug_var_a >= _debug_var_l) && (_debug_var_a <= _debug_var_r), GET_ARG_DEF2_M("%s", "Check((" #a " in [(" #l ")..(" #r ")]) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_FLOATING_POINT_EQ(a, b, ...) EXPECT_NEAR(a, b, sqrt(std::numeric_limits<typeof(a)>::epsilon()), GET_ARG_DEF2_M("%s", "Check((" #a " ~ " #b ")) failed", ## __VA_ARGS__))
#define EXPECT_FLOAT_EQ(a, b, ...) EXPECT_NEAR(a, b, sqrt(std::numeric_limits<float>::epsilon()), GET_ARG_DEF2_M("%s", "Check((" #a " ~ " #b ")) failed", ## __VA_ARGS__))
#define EXPECT_DOUBLE_EQ(a, b, ...) EXPECT_NEAR(a, b, sqrt(std::numeric_limits<double>::epsilon()), GET_ARG_DEF2_M("%s", "Check((" #a " ~ " #b ")) failed", ## __VA_ARGS__))
#define EXPECT_CSTRING_EQ(a, b, ...) \
    { \
        unsigned long i = 0; \
        while(a[i] == b[i]) \
            if(a[i] == '\0') \
                break; \
            else \
                i++; \
        TEST_INTR_EXPECT_TRUE(a[i] == b[i], GET_ARG_DEF2_M("%s", "Check strings((" #a ") == (" #b ")) failed", ## __VA_ARGS__)); \
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
            EXPECT_EQ((a)[i], (b)[i], GET_ARG_DEF2_M("%s", "Check((" #a ")[i] == (" #b ")[i], i=0,...,(" #len ")) failed", ## __VA_ARGS__)); \
    }
#define EXPECT_EQ_RA_LEN(a, b, len1, len2, ...) \
    { \
        auto len = (len1); \
        if(len != (len2)) \
            TEST_INTR_EXPECT_TRUE(false, GET_ARG_DEF2_M("%s", "Check((" #len1 ") == (" #len2 ")) failed", ## __VA_ARGS__)); \
        else \
            EXPECT_EQ_RA(a, b, len, GET_ARG_DEF2("%s", "Check((" #a ")[i] == (" #b ")[i], i=0,...,(" #len1 ") == (" #len2 ")) failed", ## __VA_ARGS__)); \
    }
#define LAST_COMPOSE_EQ_I(i, a, b) TEST_INTR_EXPECT_TRUE((a[(i)]) == (b), "Check(equals random access) failed(" QUOTE(a) "[" QUOTE(i) "] != " QUOTE(b) ")")
#define COMPOSE_EQ_I(i, a, b, ...) TEST_INTR_EXPECT_TRUE((a[(i)]) == (b), "Check(equals random access) failed(" QUOTE(a) "[" QUOTE(i) "] != " QUOTE(b) ")"); __VA_ARGS__
//EXPECT_EQ_RA_VALS(rand_access_seq, len, values...)
#define EXPECT_EQ_RA_VALS(rand_access_seq, ...) \
    { \
        if((GET_FIRST(__VA_ARGS__)) != PREV_NUM(GET_ARGS_COUNT_L(__VA_ARGS__))) \
        { \
            TEST_INTR_EXPECT_TRUE(false, "Check(random access length) failed((" QUOTE_W(GET_FIRST(__VA_ARGS__)) ") == " QUOTE_W(GET_ARGS_COUNT_L(SKIP_1L(__VA_ARGS__))) ") failed"); \
        } \
        else \
        { \
            MULT_ARG_R_N(PREV_NUM(GET_ARGS_COUNT_L(__VA_ARGS__)), COMPOSE_EQ_I, GET_3, SKIP_3_1_INC_1, 0, rand_access_seq, SKIP_1L(__VA_ARGS__)); \
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
        TEST_INTR_EXPECT_TRUE(_test_exception_check, GET_ARG_DEF2_M("%s", "Check(exception) failed", ## __VA_ARGS__)); \
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
        TEST_INTR_EXPECT_TRUE(_test_exception_check, GET_ARG_DEF2_M("%s", "Check(exception_any) failed", ## __VA_ARGS__)); \
    }
