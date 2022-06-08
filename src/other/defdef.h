#pragma once

#include "def_int.h"

#if __cplusplus >= 201703L
    #define MAYBE_UNUSED(x) [[maybe_unused]] x  // to avoid compiler warnings
#else
    #define MAYBE_UNUSED(x) (void)x  // to avoid compiler warnings
#endif

// returns first argument
#define GET_FIRST(...) D_INTR_GET_FIRST(__VA_ARGS__)
// returns second argument
#define GET_SECOND(...) D_INTR_GET_SECOND(__VA_ARGS__)

// return first three arguments
#define GET_3(a, ...) a, D_INTR_GET_FIRST(__VA_ARGS__), D_INTR_GET_SECOND(__VA_ARGS__)
#define LAST_GET_3(...) a, D_INTR_GET_FIRST(__VA_ARGS__), D_INTR_GET_SECOND(__VA_ARGS__)

// skips first argument
// at least two arguments given
#define SKIP_1E(a, ...) __VA_ARGS__
#define LAST_SKIP_1E(a, ...) __VA_ARGS__

// skips first argument
// 50 arguments limit
#define SKIP_1L(...) D_INTR_SKIP_1L(__VA_ARGS__)
#define LAST_SKIP_1L(...) D_INTR_SKIP_1L(__VA_ARGS__)

// skips first arguments
// preserves comma after first argument (arg1, args... -> , args...)
// 50 arguments limit
#define CSKIP_1L(...) D_INTR_CSKIP_1L(__VA_ARGS__)
#define LAST_CSKIP_1L(...) D_INTR_CSKIP_1L(__VA_ARGS__)

// skips first argument
// no macro arguments
#define SKIP_1M(...) D_INTR_SKIP_1M(__VA_ARGS__)
#define LAST_SKIP_1M(...) D_INTR_SKIP_1M(__VA_ARGS__)

// skips first arguments
// preserves comma after first argument (arg1, args... -> , args...)
// no macro arguments
#define CSKIP_1M(...) D_INTR_CSKIP_1M(__VA_ARGS__)
#define LAST_CSKIP_1M(...) D_INTR_CSKIP_1M(__VA_ARGS__)
        
// expands to self
#define SELF(...) __VA_ARGS__
#define LAST_SELF(...) __VA_ARGS__
    
// expands to nothing
#define EMPTY(...)

// returns quoted string from argument list
#define QUOTE(...) # __VA_ARGS__

// returns quoted string from argument list with some delay for arguments expansion
#define QUOTE_W(...) D_INTR_QUOTE_W(__VA_ARGS__)

// call another macro function 'f' with 'args' arguments
// args must be enclosed in brackets (f.e.: args = '(0, 1, 42)'
#define CALL(f, args) f args
#define CALL_I(f, args) f args

// concatenates two arguments
#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)

// emulates GNU C++ extension behaviour for , ## __VA_ARGS__
// if __VA_ARGS__ is empty, then expands to nothing
// if __VA_ARGS__ is not empty, then expands to ", __VA_ARGS__"
// no macro in arguments allowed
#define GNU_VA_ARGS_COMMA(...) D_INTR_CE_M(__VA_ARGS__)

// returns amount of arguments in argument list
// GET_ARGS_COUNT(args...) -> int
// 50 arguments limit
#define GET_ARGS_COUNT_L(...) D_INTR_TAKE53(53, 52, __VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
                                                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// recursive(N <= 50: up to 50 levels!) macro to expand
// result = compose(get_current_args(args...),
//              compose(get_current_args(get_args_next(args...)),
//                  compose(...))), where last compose is: LAST_ ## compose(args...) (without get_current_args!)
// NOTE: don't start compose name with underscore as names with leading (and double) underscore is reserved
// MULT_ARG_R_N(N, compose, get_current_args, get_args_next, args...)
// expands to nothing if N == 0
#define MULT_ARG_R_N(N, ...) D_INTR_MULT_ARG_R_N(N, __VA_ARGS__)

// same as MULT_ARG_R_N with N = amount of args
#define MULT_ARG_R_C(compose, get_current, get_arg_next, ...) \
    MULT_ARG_R_N(D_INTR_GET_ARGS_COUNT_L(__VA_ARGS__), compose, get_current, get_arg_next, __VA_ARGS__)

// multiply given arguments list N times, N >= 0 && N <= 50
// expands to nothing if N == 0
#define MULT_ARG(N, ...) D_INTR_CONCAT(D_INTR_MULT_ARG_, N)(__VA_ARGS__)

// multiply given arguments list N times and put comma in the end, N >= 0 && N <= 50
// expands to nothing if N == 0
#define MULT_ARGC(N, ...) D_INTR_CONCAT(D_INTR_MULT_ARGC_, N)(__VA_ARGS__)

// arguments multiplied 50 - N times, N >= 0 && N <= 50
#define MULT_ARG_50_MN(N, ...) MULT_ARG(D_INTR_50_MN(N), __VA_ARGS__)

// returns Nth argument from given argument list
// NOTE: N <= 50
// GET_ARG_N(N, args...) -> args[N]
#define GET_ARG_N(...) D_INTR_GET_ARG_N(__VA_ARGS__)

// returns arguments without last
// 50 arguments limit
#define EXCLUDE_LAST_L(...) D_INTR_EXCLUDE_LAST_L(__VA_ARGS__)

// returns last argument from list
// 50 arguments limit
#define GET_LAST_L(...) D_INTR_GET_ARG_N(D_INTR_GET_ARGS_COUNT_L(__VA_ARGS__), D_INTR_DUMMY_ARG, __VA_ARGS__)

// returns 50 - N, N >= 0 && N <= 50
#define SUB_FROM_50(N) D_INTR_50_MN(N)

// returns N - 1, N > 0 && N <= 50
#define PREV_NUM(N) D_INTR_PREV(N)

// returns N + 1, N >= 0 && N <= 50
#define NEXT_NUM(N) D_INTR_NEXT(N)

// compares A and B, A >= 0 && A <= 50, B >= 0 && B <= 50
// if A < B, returns L
// if A == B, returns E
// if A > B, returns G
#define COMPARE_NUM(A, B) D_INTR_CMP(A, B)
    
// returns def if only def is provided, otherwise returns all arguments after def
// 50 arguments limit
// GET_ARG_DEF_L(def[, args...]) -> if len(args) > 0 returns args, otherwise returns def
#define GET_ARG_DEF_L(...) D_INTR_GET_ARG_DEF_L(__VA_ARGS__)

// returns def if only def is provided, otherwise returns all arguments after def
// no macro arguments
// GET_ARG_DEF_M(def[, args...]) -> if len(args) > 0 returns args, otherwise returns def
#define GET_ARG_DEF_M(...) D_INTR_GET_ARG_DEF_M(__VA_ARGS__)

// returns def1, def2 if they are only arguments provided, otherwise returns all arguments after def2
// 50 arguments limit
// GET_ARG_DEF2_L(def1, def2[, args...])
#define GET_ARG_DEF2_L(def1, ...) D_INTR_GET_ARG_DEF2_L(def1, __VA_ARGS__)

// returns def if only def is provided, otherwise returns all arguments after def
// no macro arguments
// GET_ARG_DEF2_M(def1, def2[, args...])
#define GET_ARG_DEF2_M(def1, ...) D_INTR_GET_ARG_DEF2_M(def1, __VA_ARGS__)

// returns function(args...) if some args is provided
// if no args provided expands to nothing
// 50 arguments limit
// FCALL_IF_ARGS_L(N, function[, args...])
#define FCALL_IF_ARGS_L(...) D_INTR_FCALL_IF_ARGS_L(__VA_ARGS__)

// returns function(args...) if some args is provided
// if no args provided expands to nothing
// no macro arguments
// FCALL_IF_ARGS_M(N, function[, args...])
#define FCALL_IF_ARGS_M(...) D_INTR_FCALL_IF_ARGS_M(__VA_ARGS__)

// returns function(args...) if amount of arguments more than N, N < 50
// 49 arguments limit
// FCALL_IF_ARGS_GT_N_L(N, function[, args...])
#define FCALL_IF_ARGS_GT_N_L(N, ...) D_INTR_FCALL_IF_ARGS_GT_N_L(N, __VA_ARGS__)

// if only 1 argument is provided and it in braces than removes outer braces, otherwise returns arguments intact
// 49 arguments limit
#define FREE_L(...) D_INTR_FREE_L(__VA_ARGS__)

// if only 1 argument is provided and it in braces than removes outer braces, otherwise returns arguments intact
// no macro arguments
#define FREE_M(...) D_INTR_FREE_M(__VA_ARGS__)

// returns arguments list in reversed order
#define REVERSE_L(...) D_INTR_REVERSE_L(__VA_ARGS__)

// returns a sequence of length N, which consists of powers of 10 (1, 10, 100, ...)
// N >= 0 && N <= 50
#define SEQ_10_POW(N) D_INTR_SEQ_10_POW(N)
