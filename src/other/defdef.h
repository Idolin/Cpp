#pragma once

#include "def_int.h"

#define UNUSED(x) (void)x  // to avoid removing variable by compiler

// returns amount of arguments in argument list
#define GET_ARGS_COUNT(...) D_INTR_TAKE53(53, 52, ## __VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
                                                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
        
#define GET_FIRST(a, ...) a
#define GET_SECOND(a, b, ...) b

#define GET_3(a, b, c, ...) a, b, c
#define LAST_GET_3(...) GET_3(__VA_ARGS__)

#define SKIP_1(a, ...) __VA_ARGS__    
#define LAST_SKIP_1(...) SKIP_1(__VA_ARGS__)
        
#define SELF(...) __VA_ARGS__
#define LAST_SELF(...) __VA_ARGS__
    
#define EMPTY(...)

#define QUOTE(...) D_INTR_QUOTE(__VA_ARGS__) //returns quoted string from argument list
        
#define CALL(f, arg) f arg // call another macro function where arg is (args...)
#define CALL_I(f, arg) f arg
    
#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)

#define MULT_ARG_R_N_(N, compose, get_current, get_arg_next, ...) \
    D_INTR_CONCAT(MULT_ARG_R_, N)(compose, get_current, get_arg_next, ## __VA_ARGS__)

// recursive(50 levels!) macro to expand
// result = compose(get_current(args...), compose(get_current(get_arg_next(args...)),
//  compose(...))), where last compose is: LAST_ ## compose(args...)
// NOTE: don't start compose name with underscore as names with double underscore is reserved
#define MULT_ARG_R_N(N, compose, get_current, get_arg_next, ...) \
    MULT_ARG_R_N_(N, compose, get_current, get_arg_next, ## __VA_ARGS__)

#define MULT_ARG_R_C(compose, get_current, get_arg_next, ...) \
    MULT_ARG_R_N(D_INTR_GET_ARGS_COUNT(__VA_ARGS__), compose, get_current, get_arg_next, ## __VA_ARGS__)

// multiply given argument list N times
#define MULT_ARG(N, ...) \
    D_INTR_CONCAT(MULT_ARG_R_, N)(D_INTR_SELF, D_INTR_SELF, D_INTR_SELF, ## __VA_ARGS__)

// returns Nth argument from given argument list
#define GET_ARG_N(N, ...) \
    CONCAT(MULT_ARG_R_, N)(D_INTR_LSKIP_1, D_INTR_GET_FIRST, D_INTR_SKIP_1, ## __VA_ARGS__)

// returns arguments without last
#define EXCLUDE_LAST(...) \
    MULT_ARG_R_C(D_INTR_LSELF, D_INTR_GET_FIRST, D_INTR_SKIP_1, ## __VA_ARGS__)

// returns last argument from list
#define GET_LAST(...) D_INTR_GET_ARG_N(D_INTR_GET_ARGS_COUNT(__VA_ARGS__), __VA_ARGS__)
    
// returns def if only def is provided, otherwise returns all arguments after def
#define GET_ARG_DEF(def, ...) D_INTR_CONCAT(D_INTR_GET_ARG_DEF_, D_INTR_GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)
// another GET_ARG_DEF(to use inside GET_ARG_DEF)
#define GET_ARG_DEF_I(def, ...) D_INTR_CONCAT(D_INTR_GET_ARG_DEF_, D_INTR_GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)
// another GET_ARG_DEF(to use inside GET_ARG_DEF_I)
#define GET_ARG_DEF_II(def, ...) D_INTR_CONCAT(D_INTR_GET_ARG_DEF_, D_INTR_GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)

// returns def1, def2 if they are only arguments provided, otherwise returns all arguments after def2
#define GET_ARG_DEF2(def1, def2, ...) D_INTR_CONCAT(D_INTR_GET_ARG_DEF2_, D_INTR_GET_ARGS_COUNT_3(def2, ## __VA_ARGS__))(def1, def2, ## __VA_ARGS__)
// another GET_ARG_DEF2(to use inside GET_ARG_DEF2)
#define GET_ARG_DEF2_I(def1, def2, ...) D_INTR_CONCAT(D_INTR_GET_ARG_DEF2_, D_INTR_GET_ARGS_COUNT_3(def2, ## __VA_ARGS__))(def1, def2, ## __VA_ARGS__)
// another GET_ARG_DEF2(to use inside GET_ARG_DEF2_I)
#define GET_ARG_DEF2_II(def1, def2, ...) D_INTR_CONCAT(D_INTR_GET_ARG_DEF2_, D_INTR_GET_ARGS_COUNT_3(def2, ## __VA_ARGS__))(def1, def2, ## __VA_ARGS__)

// returns function(args...) if amount of arguments more then zero, returns nothing otherwise
#define START_IF_ARGS(function, ...) D_INTR_CONCAT(D_INTR_START_IF_ARGS_, D_INTR_GET_ARGS_COUNT_2(__VA_ARGS__))(function, ## __VA_ARGS__)

// arguments multiplied 50 - N times
#define MULT_ARG_50_MN(N, ...) \
    MULT_ARG(D_INTR_50MN(N), ## __VA_ARGS__)

// returns amount of arguments - N
#define GET_ARGS_COUNT_MN(N, ...) D_INTR_SELF(D_INTR_TAKE53 D_INTR_SELF() (1, ## __VA_ARGS__, D_INTR_MULT_ARG_50_MN(N, 1), 1, 0, 0, 0))

// returns function(args...) if amount of arguments more than N
#define START_IF_ARGS_NDEF(function, N, ...) D_INTR_CONCAT(D_INTR_START_IF_ARGS_NDEF_, D_INTR_GET_ARGS_COUNT_MN(N, ## __VA_ARGS__))(function, ## __VA_ARGS__)

// if argument is in braces than removes outer braces, otherwise returns arguments intact
#define FREE(...) D_INTR_CONCAT(D_INTR_FREE_, D_INTR_F_CALL(D_INTR_GET_ARGS_COUNT_2, (D_INTR_FCONCAT(__VA_ARGS__))))(__VA_ARGS__)

#define QUOTE_F(...) D_INTR_QUOTE(D_INTR_FREE(__VA_ARGS__)) // QUOTE(FREE(args...))

#define FOR_EACH_(compose, before, after, ...) \
    { \
        MULT_ARG_R_N(D_INTR_GET_ARGS_COUNT(__VA_ARGS__), compose, D_INTR_GET_3, D_INTR_SKIP_1_3, before, after, ## __VA_ARGS__) \
    }

// returns MULT_ARG_R_N sequence compose(before, after, arg1, compose(...))
#define FOR_EACH(compose, before, after, ...) FOR_EACH_(compose, before, after, __VA_ARGS__)

// FOR_EACH_ARG(type name, values to iterate, ({ body }))
// returns sequence {var_decl = arg1; { body }} ... for every arg in args...
// (except body which is last argument in args...)
#define FOR_EACH_ARG(var_decl, ...) \
    FOR_EACH(D_INTR_COMPOSE_FOR_EACH, var_decl =, \
    ; D_INTR_GET_LAST(__VA_ARGS__), EXCLUDE_LAST(__VA_ARGS__))

// ({body}) - last argument
// returns MULT_ARG_R_N sequence compose(var_decl =, arg1, ({ body }), compose(...))
#define FOR_EACH_ARG_COMPOSE(compose, var_decl, ...) \
    FOR_EACH(compose, var_decl =, \
    ; D_INTR_GET_LAST(__VA_ARGS__), EXCLUDE_LAST(__VA_ARGS__))
    
// returns arguments list in reversed order
#define REVERSE(...) D_INTR_REVERSE(__VA_ARGS__)

// returns sequence S_n where s_i = i times applied operation on init, 0 <= i < length
#define SEQUENCE(init, operation, length) \
    REVERSE(MULT_ARG_R_N(length, D_INTR_COMPOSE_SEQUENCE, D_INTR_SELF, D_INTR_SELF, init, operation))

// returns sequence of T_n where t_i = 10^i, 0 <= i < length
#define SEQ_10_POW(N) REVERSE(MULT_ARG_R_N(N, D_INTR_POW_10, D_INTR_EMPTY, D_INTR_EMPTY))

// return N + 1(for N < 50)
#define NEXT(N) D_INTR_SELF(D_INTR_GET_ARGS_COUNT D_INTR_SELF() (0 D_INTR_CE_I(D_INTR_MULT_ARG_N(N, 0))))
