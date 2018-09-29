#pragma once

#define elif else if
#define smin_(a, b) (a = ((a < b) ? (a) : (b)))
#define smax_(a, b) (a = ((a < b) ? (b) : (a)))

#define UNUSED(x) (void)x  //to avoid removing variable by compiler

#define TAKE53(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                 _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
                 _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, ...) _53

//returns amount of arguments in argument list
#define GET_ARGS_COUNT(...) TAKE53(53, 52, ## __VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
                                                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define GET_FIRST(_1, ...) _1
#define GET_SECOND(_1, _2, ...) _2
#define SELF(...) __VA_ARGS__
#define EMPTY(...)

#define CE_(...) , ## __VA_ARGS__
#define CE(...) CE_(__VA_ARGS__) //put comma before non-empty list of arguments
#define CALL(f, arg) f arg //call another macro function where arg is (args...)

#define MS_(...) __VA_ARGS__
#define MS(...) MS_(__VA_ARGS__) //another SELF macros

//returns 0 on zero amount of arguments and 1 otherwise
#define GET_ARGS_COUNT_2(...) SELF(TAKE53 SELF() (1, ## __VA_ARGS__, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0))

#define MC_(a, b) a ## b
#define MC(a, b) MC_(a, b) //another CONCAT macros

#define MSS(...) __VA_ARGS__  //just another SELF macros

#define CEE_0 MS
#define CEE_1 CE
//returns CE on zero amount of arguments or MS otherwise
#define CEE(...) MSS(__VA_ARGS__)MC(CEE_, GET_ARGS_COUNT_2(__VA_ARGS__))

//CEE shouldn't influence on macro expansion except for the extra spaces after commas
#define MULT_ARG_R_0(C, G, N, ...)
#define MULT_ARG_R_1(C, G, N, ...) CALL(_LAST_ ## C, (__VA_ARGS__))
#define MULT_ARG_R_2(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_1(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_3(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_2(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_4(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_3(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_5(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_4(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_6(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_5(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_7(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_6(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_8(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_7(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_9(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_8(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_10(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_9(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_11(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_10(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_12(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_11(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_13(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_12(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_14(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_13(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_15(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_14(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_16(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_15(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_17(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_16(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_18(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_17(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_19(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_18(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_20(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_19(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_21(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_20(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_22(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_21(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_23(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_22(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_24(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_23(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_25(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_24(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_26(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_25(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_27(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_26(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_28(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_27(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_29(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_28(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_30(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_29(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_31(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_30(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_32(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_31(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_33(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_32(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_34(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_33(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_35(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_34(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_36(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_35(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_37(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_36(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_38(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_37(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_39(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_38(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_40(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_39(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_41(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_40(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_42(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_41(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_43(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_42(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_44(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_43(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_45(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_44(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_46(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_45(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_47(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_46(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_48(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_47(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_49(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_48(C, G, N, N(__VA_ARGS__)))))
#define MULT_ARG_R_50(C, G, N, ...) \
    CALL(C,(CEE(G(__VA_ARGS__))(MULT_ARG_R_49(C, G, N, N(__VA_ARGS__)))))

#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)

#define MULT_ARG_R_N_(N, compose, get_current, get_arg_next, ...) \
    CONCAT(MULT_ARG_R_, N)(compose, get_current, get_arg_next, ## __VA_ARGS__)

//recursive(50 levels!) macro to expand
//result = compose(get_current(args...), compose(get_current(get_arg_next(args...)),
//  compose(...))), where last compose is: _LAST_ ## compose(args...)
#define MULT_ARG_R_N(N, compose, get_current, get_arg_next, ...) \
    MULT_ARG_R_N_(N, compose, get_current, get_arg_next, ## __VA_ARGS__)

#define MULT_ARG_R_C(compose, get_current, get_arg_next, ...) \
    MULT_ARG_R_N(GET_ARGS_COUNT(__VA_ARGS__), compose, get_current, get_arg_next, ## __VA_ARGS__)

#define _LAST_SELF(...) __VA_ARGS__

//multiply given argument list N times
#define MULT_ARG(N, ...) \
    CONCAT(MULT_ARG_R_, N)(SELF, SELF, SELF, ## __VA_ARGS__)

#define _LAST_LSKIP1(a, ...) a

#define LSKIP1(a, ...) __VA_ARGS__

//returns Nth argument from given argument list
#define GET_ARG_N(N, ...) \
    CONCAT(MULT_ARG_R_, N)(LSKIP1, GET_FIRST, SKIP1, ## __VA_ARGS__)

#define SKIP1(B, ...) __VA_ARGS__
#define _LAST_SKIP1(...) SKIP1(__VA_ARGS__)

#define _LAST_LSELF(...)
#define LSELF(...) __VA_ARGS__

//returns arguments without last
#define EXCLUDE_LAST(...) \
    MULT_ARG_R_C(LSELF, GET_FIRST, SKIP1, ## __VA_ARGS__)

#define _QUOTE(...) # __VA_ARGS__
#define QUOTE(...) _QUOTE(__VA_ARGS__) //returns quoted string from argument list

#define GET_ARGS_COUNT_3(...) SELF(TAKE53 SELF() (2, ## __VA_ARGS__, \
    MULT_ARG(50, 2), 1, 0)) //if amount of arguments is zero returns 0,
    // if 1 then returns 1, otherwise returns 2

//returns last argument from list
#define GET_LAST(...) GET_ARG_N(GET_ARGS_COUNT(__VA_ARGS__), __VA_ARGS__)
#define _GET_ARG_DEF__0() _NO_SUCH_ARG_
#define _GET_ARG_DEF__1(a) a
#define _GET_ARG_DEF__2(a, ...) __VA_ARGS__
//returns def if only def is provided, otherwise returns all arguments after def
#define GET_ARG_DEF(def, ...) CONCAT(_GET_ARG_DEF__, GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)
//another GET_ARG_DEF(to use inside GET_ARG_DEF)
#define GET_ARG_DEF_I(def, ...) CONCAT(_GET_ARG_DEF__, GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)
//another GET_ARG_DEF(to use inside GET_ARG_DEF_I)
#define GET_ARG_DEF_II(def, ...) CONCAT(_GET_ARG_DEF__, GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)

#define _START_IF_ARGS__0(f)
#define _START_IF_ARGS__1(f, ...) f(__VA_ARGS__)
//returns function(args...) if amount of arguments more then zero, returns nothing otherwise
#define START_IF_ARGS(function, ...) CONCAT(_START_IF_ARGS__, GET_ARGS_COUNT_2(__VA_ARGS__))(function, ## __VA_ARGS__)

//returns 50 - N
#define _50MN(N) SELF(TAKE53 SELF() (<?>, <?>, <?> CE(MULT_ARG(N, 0)), 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, \
    28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, \
    45, 46, 47, 48, 49, 50))

//multiply arguments multiplied 50 - N amount of times
#define MULT_ARG_50_MN(N, ...) \
    MULT_ARG(_50MN(N), ## __VA_ARGS__)

//returns amount of arguments - N
#define GET_ARGS_COUNT_2MN(N, ...) SELF(TAKE53 SELF() (1, ## __VA_ARGS__, MULT_ARG_50_MN(N, 1), 1, 0, 0, 0))
#define _START_IF_ARGS_NDEF__0(...)
#define _START_IF_ARGS_NDEF__1(f, ...) f(__VA_ARGS__)
#define _START_IF_ARGS_NDEF__2(f, ...) _START_IF_ARGS_NDEF__1(f, ## __VA_ARGS__)
//returns function(args...) if amount of arguments more than N
#define START_IF_ARGS_NDEF(function, N, ...) CONCAT(_START_IF_ARGS_NDEF__, GET_ARGS_COUNT_2MN(N, ## __VA_ARGS__))(function, ## __VA_ARGS__)

#define FREE_1(...) __VA_ARGS__
#define FREE_0(X) SELF X
#define __FCONCAT(E, ...) E __VA_ARGS__
#define _FCONCAT(...) __FCONCAT(EMPTY, ## __VA_ARGS__)
#define CALL_I(f, arg) f arg //another CALL
//if argument is in braces than removes outer braces, otherwise returns arguments intact
#define FREE(...) CONCAT(FREE_, CALL_I(GET_ARGS_COUNT_2, (_FCONCAT(__VA_ARGS__))))(__VA_ARGS__)

#define QUOTE_F(...) QUOTE(FREE(__VA_ARGS__)) //QUOTE(FREE(args...))

#define _LAST_COMPOSE_FOR_EACH(B, A, X) { B X FREE(A) }
#define COMPOSE_FOR_EACH(B, A, X, ...) { B X FREE(A) } __VA_ARGS__

#define GET_3FIRST(a, b, c, ...) a, b, c
#define SKIP3_1(a, b, skip, ...) a, b, ## __VA_ARGS__

#define FOR_EACH_(compose, before, after, ...) \
    { \
        MULT_ARG_R_N(GET_ARGS_COUNT(__VA_ARGS__), compose, GET_3FIRST, SKIP3_1, before, after, ## __VA_ARGS__) \
    }

//returns MULT_ARG_R_N sequence compose(before, after, arg1, compose(...))
#define FOR_EACH(compose, before, after, ...) FOR_EACH_(compose, before, after, __VA_ARGS__)


//FOR_EACH_ARG(type name, values to iterate, ({ body }))
//returns sequence {var_decl = arg1; { body }} ... for every arg in args...
// (except body which is last argument in args...)
#define FOR_EACH_ARG(var_decl, ...) \
    FOR_EACH(COMPOSE_FOR_EACH, var_decl =, \
    ; GET_LAST(__VA_ARGS__), EXCLUDE_LAST(__VA_ARGS__))

//({body}) - last argument
//returns MULT_ARG_R_N sequence compose(var_decl =, arg1, ({ body }), compose(...))
#define FOR_EACH_ARG_COMPOSE(compose, var_decl, ...) \
    FOR_EACH(compose, var_decl =, \
    ; GET_LAST(__VA_ARGS__), EXCLUDE_LAST(__VA_ARGS__))

#define _LAST_REV_COMPOSE(a) a
#define REV_COMPOSE(a, ...) __VA_ARGS__, a

//returns arguments list in reversed order
#define REVERSE_(...) \
    MULT_ARG_R_N(GET_ARGS_COUNT(__VA_ARGS__), REV_COMPOSE, GET_FIRST, SKIP1, ## __VA_ARGS__)
#define REVERSE(...) REVERSE_(__VA_ARGS__)

#define _LAST_COMPOSE_SEQUENCE(init, operation) (init)
#define COMPOSE_SEQUENCE(init, operation, sequence, ...) (sequence operation), sequence, ## __VA_ARGS__

//returns sequence S_n where s_i = i times applied operation on init, 0 <= i < length
#define SEQUENCE(init, operation, length) \
    REVERSE(MULT_ARG_R_N(length, COMPOSE_SEQUENCE, SELF, SELF, init, operation))

#define _LAST_POW_10() 1
#define POW_10(a, ...) a ## 0, a, ## __VA_ARGS__

//returns sequence of T_n where t_i = 10^i, 0 <= i < length
#define SEQ_10_POW(N) REVERSE(MULT_ARG_R_N(N, POW_10, EMPTY, EMPTY))