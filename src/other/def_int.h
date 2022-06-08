#pragma once

/* WARNING: DARK MACROMAGIC */

// prefix D_INTR_ is used for internal macros

// D_INTR_DUMMY_ARG mostly used to fullfil C++11 - C++17 requirement to pass at least one argument in ellipsis(...) in variadic macro

// expands to self
#define D_INTR_SELF(...) __VA_ARGS__
#define LAST_D_INTR_SELF(...) __VA_ARGS__

// calls another macro function f with arguments args (arguments must be enclosed шт parenthesis's)
#define D_INTR_CALL_GET(f, args) f args
#define D_INTR_CALL_TAKE(f, args) f args
#define D_INTR_CALL_SKIP(f, args) f args
#define D_INTR_CALL_FEW(f, args) f args
#define D_INTR_CALL_FEW_I(f, args) f args
#define D_INTR_CALL_ARGS2(f, args) f args
#define D_INTR_CALL_MULT_R(f, args) f args
#define D_INTR_CALL_MULT_R_N(f, args) f args
#define D_INTR_CALL_MULT_ARG_N(f, args) f args
#define D_INTR_CALL_MULT_ARGC_N(f, args) f args
#define D_INTR_CALL_GETN(f, args) f args
#define D_INTR_CALL_DEF(f, args) f args
#define D_INTR_CALL_DEF2(f, args) f args
#define D_INTR_CALL_ARGS(f, args) f args
#define D_INTR_CALL_ARGS_GT(f, args) f args
#define D_INTR_CALL_FREE(f, args) f args



// self macro with some delay
#define D_INTR_W_(...) __VA_ARGS__
#define D_INTR_W(...) D_INTR_W_(__VA_ARGS__)
#define D_INTR_W_I_(...) __VA_ARGS__
#define D_INTR_W_I(...) D_INTR_W_I_(__VA_ARGS__)

// call macro with delay
#define D_INTR_CALL_W(f, args) D_INTR_W(f D_INTR_W() args)
#define D_INTR_CALL_W_I(f, args) D_INTR_W(f D_INTR_W_I() args)

// getting first argument
#define D_INTR_GET_FIRST_(a1, ...) a1
#define D_INTR_GET_FIRST(...) D_INTR_CALL_GET(D_INTR_GET_FIRST_, (__VA_ARGS__, D_INTR_DUMMY_ARG))

// getting second argument
#define D_INTR_GET_SECOND_(a1, a2, ...) a2
#define D_INTR_GET_SECOND(...) D_INTR_CALL_GET(D_INTR_GET_SECOND_, (__VA_ARGS__, D_INTR_NO_SUCH_ARG, D_INTR_DUMMY_ARG))

// CONCAT macro (concatenates two arguments)
#define D_INTR_CONCAT_(a, b) a ## b
#define D_INTR_CONCAT(a, b) D_INTR_CONCAT_(a, b)
#define D_INTR_CONCAT_I_(a, b) a ## b
#define D_INTR_CONCAT_I(a, b) D_INTR_CONCAT_I_(a, b)

// CONCAT macro with delay before concatenation (waiting for arguments to expand)
#define D_INTR_CONCAT_W_(a, b) a ## b
#define D_INTR_CONCAT_W(a, b) D_INTR_CALL_W(D_INTR_CONCAT_W_, (a, b))

// returns 53 argument
#define D_INTR_TAKE53_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, ...) _53
#define D_INTR_TAKE53(...) D_INTR_CALL_TAKE(D_INTR_TAKE53_, (__VA_ARGS__, D_INTR_DUMMY_ARG))

// return amount of arguments
#define D_INTR_GET_ARGS_COUNT_L(...) D_INTR_TAKE53(53, 52, __VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
                                                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// returns 1 on zero or one argument and 2 otherwise
#define D_INTR_GET_ARGS_COUNT_FEW2L(...) D_INTR_TAKE53(2, __VA_ARGS__, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1)

#define D_INTR_SKIP_1L_1(...) // was given one argument, thus returning nothing
#define D_INTR_SKIP_1L_2(a, ...) __VA_ARGS__
#define D_INTR_SKIP_1L(...) D_INTR_CALL_SKIP(D_INTR_CONCAT, (D_INTR_SKIP_1L_, D_INTR_GET_ARGS_COUNT_FEW2L(__VA_ARGS__))(__VA_ARGS__))

#define D_INTR_CSKIP_1L_1(...) // was given one argument, thus returning nothing
#define D_INTR_CSKIP_1L_2(a, ...) , __VA_ARGS__
#define D_INTR_CSKIP_1L(...) D_INTR_CALL_SKIP(D_INTR_CONCAT, (D_INTR_CSKIP_1L_, D_INTR_GET_ARGS_COUNT_FEW2L(__VA_ARGS__))(__VA_ARGS__))

#define D_INTR_TAKE3(_1, _2, _3, ...) _3 // returns 3rd argument
#define D_INTR_TAKE4(_1, _2, _3, _4, ...) _4
#define D_INTR_EXP2(...) D_INTR_DUMMY_ARG, D_INTR_DUMMY_ARG // expands to two arguments
#define D_INTR_EXP2_EXP2(...) D_INTR_DUMMY_ARG, D_INTR_EXP2 // expands to two arguments

// returns 1 on zero or one argument and 2 otherwise
#define D_INTR_GET_ARGS_COUNT_FEW2M_(a, m, ...) D_INTR_CALL_FEW_I(D_INTR_TAKE3, (m(), 1, 2, D_INTR_DUMMY_ARG))
#define D_INTR_GET_ARGS_COUNT_FEW2M(...) D_INTR_CALL_FEW(D_INTR_GET_ARGS_COUNT_FEW2M_, (__VA_ARGS__, D_INTR_EXP2, D_INTR_DUMMY_ARG))

#define D_INTR_SKIP_1M_1(...) // was given one argument, thus returning nothing
#define D_INTR_SKIP_1M_2(a, ...) __VA_ARGS__
#define D_INTR_SKIP_1M(...) D_INTR_CONCAT_W(D_INTR_SKIP_1M_, D_INTR_GET_ARGS_COUNT_FEW2M(__VA_ARGS__))(__VA_ARGS__)

#define D_INTR_CSKIP_1M_1(...) // was given one argument, thus returning nothing
#define D_INTR_CSKIP_1M_2(a, ...) , __VA_ARGS__
#define D_INTR_CSKIP_1M(...) D_INTR_CONCAT_W(D_INTR_CSKIP_1M_, D_INTR_GET_ARGS_COUNT_FEW2M(__VA_ARGS__))(__VA_ARGS__)

#define D_INTR_SKIP_1E_(a, ...) __VA_ARGS__
#define D_INTR_SKIP_1E(...) D_INTR_CALL_SKIP(D_INTR_SKIP_1E_, (__VA_ARGS__))

#define D_INTR_QUOTE_W_(...) # __VA_ARGS__
#define D_INTR_QUOTE_W(...) D_INTR_CALL_W_I(D_INTR_QUOTE_W_, (__VA_ARGS__))

#define D_INTR_GET_ARGS_COUNT_2_N0(...) D_INTR_CALL_ARGS2(D_INTR_TAKE4, (__VA_ARGS__, 1, 0, 1, D_INTR_DUMMY_ARG))

// __VA_ARGS__ in D_INTR_GET_ARGS_COUNT_2 contains 1 argument, so we are checking whether it's empty or not
#define D_INTR_GET_ARGS_COUNT_2_1(...) D_INTR_GET_ARGS_COUNT_2_N0(D_INTR_EXP2_EXP2 __VA_ARGS__ ())

// __VA_ARGS__ in D_INTR_GET_ARGS_COUNT_2 contains 2 or more arguments
#define D_INTR_GET_ARGS_COUNT_2_2(...) 1

// returns 0 on zero amount of arguments and 1 otherwise
// if __VA_ARGS__ contains 1 argument calls D_INTR_GET_ARGS_COUNT_2_0 to check whether it's empty(to return 0) or not
#define D_INTR_GET_ARGS_COUNT_2(...) D_INTR_CONCAT_W(D_INTR_GET_ARGS_COUNT_2_, D_INTR_GET_ARGS_COUNT_FEW2M_(__VA_ARGS__, D_INTR_EXP2, D_INTR_DUMMY_ARG))(__VA_ARGS__)

#define D_INTR_CE_0(...) __VA_ARGS__
#define D_INTR_CE_1(...) , __VA_ARGS__
#define D_INTR_CE_M(...) D_INTR_CONCAT(D_INTR_CE_, D_INTR_GET_ARGS_COUNT_2(__VA_ARGS__))(__VA_ARGS__) // put comma before non-empty list of arguments

// D_INTR_MULT_ARG_R_X(compose, get_current_args, get_args_next, args...) ->
//  compose(get_current_args(args...), compose(get_current_args(get_args_next(args...)), compose(...)))
#define D_INTR_MULT_ARG_R_0(C, G, N, ...)
#define D_INTR_MULT_ARG_R_1(C, G, N, ...) D_INTR_CALL_MULT_R(LAST_ ## C, (__VA_ARGS__))
#define D_INTR_MULT_ARG_R_2(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_1(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_3(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_2(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_4(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_3(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_5(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_4(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_6(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_5(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_7(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_6(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_8(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_7(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_9(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_8(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_10(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_9(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_11(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_10(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_12(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_11(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_13(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_12(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_14(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_13(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_15(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_14(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_16(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_15(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_17(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_16(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_18(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_17(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_19(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_18(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_20(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_19(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_21(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_20(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_22(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_21(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_23(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_22(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_24(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_23(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_25(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_24(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_26(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_25(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_27(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_26(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_28(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_27(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_29(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_28(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_30(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_29(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_31(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_30(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_32(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_31(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_33(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_32(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_34(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_33(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_35(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_34(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_36(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_35(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_37(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_36(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_38(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_37(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_39(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_38(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_40(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_39(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_41(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_40(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_42(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_41(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_43(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_42(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_44(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_43(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_45(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_44(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_46(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_45(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_47(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_46(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_48(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_47(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_49(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_48(C, G, N, N(__VA_ARGS__))))
#define D_INTR_MULT_ARG_R_50(C, G, N, ...) \
    D_INTR_CALL_MULT_R(C, (G(__VA_ARGS__), D_INTR_MULT_ARG_R_49(C, G, N, N(__VA_ARGS__))))

#define D_INTR_MULT_ARG_R_N(N, ...) D_INTR_CALL_MULT_R_N(D_INTR_CONCAT(D_INTR_MULT_ARG_R_, N), (__VA_ARGS__))

// multiply arguments N times
#define D_INTR_MULT_ARG_0(...)
#define D_INTR_MULT_ARG_1(...) __VA_ARGS__
#define D_INTR_MULT_ARG_2(...) D_INTR_MULT_ARG_1(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_3(...) D_INTR_MULT_ARG_2(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_4(...) D_INTR_MULT_ARG_3(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_5(...) D_INTR_MULT_ARG_4(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_6(...) D_INTR_MULT_ARG_5(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_7(...) D_INTR_MULT_ARG_6(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_8(...) D_INTR_MULT_ARG_7(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_9(...) D_INTR_MULT_ARG_8(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_10(...) D_INTR_MULT_ARG_9(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_11(...) D_INTR_MULT_ARG_10(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_12(...) D_INTR_MULT_ARG_11(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_13(...) D_INTR_MULT_ARG_12(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_14(...) D_INTR_MULT_ARG_13(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_15(...) D_INTR_MULT_ARG_14(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_16(...) D_INTR_MULT_ARG_15(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_17(...) D_INTR_MULT_ARG_16(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_18(...) D_INTR_MULT_ARG_17(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_19(...) D_INTR_MULT_ARG_18(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_20(...) D_INTR_MULT_ARG_19(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_21(...) D_INTR_MULT_ARG_20(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_22(...) D_INTR_MULT_ARG_21(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_23(...) D_INTR_MULT_ARG_22(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_24(...) D_INTR_MULT_ARG_23(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_25(...) D_INTR_MULT_ARG_24(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_26(...) D_INTR_MULT_ARG_25(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_27(...) D_INTR_MULT_ARG_26(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_28(...) D_INTR_MULT_ARG_27(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_29(...) D_INTR_MULT_ARG_28(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_30(...) D_INTR_MULT_ARG_29(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_31(...) D_INTR_MULT_ARG_30(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_32(...) D_INTR_MULT_ARG_31(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_33(...) D_INTR_MULT_ARG_32(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_34(...) D_INTR_MULT_ARG_33(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_35(...) D_INTR_MULT_ARG_34(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_36(...) D_INTR_MULT_ARG_35(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_37(...) D_INTR_MULT_ARG_36(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_38(...) D_INTR_MULT_ARG_37(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_39(...) D_INTR_MULT_ARG_38(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_40(...) D_INTR_MULT_ARG_39(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_41(...) D_INTR_MULT_ARG_40(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_42(...) D_INTR_MULT_ARG_41(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_43(...) D_INTR_MULT_ARG_42(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_44(...) D_INTR_MULT_ARG_43(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_45(...) D_INTR_MULT_ARG_44(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_46(...) D_INTR_MULT_ARG_45(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_47(...) D_INTR_MULT_ARG_46(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_48(...) D_INTR_MULT_ARG_47(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_49(...) D_INTR_MULT_ARG_48(__VA_ARGS__), __VA_ARGS__
#define D_INTR_MULT_ARG_50(...) D_INTR_MULT_ARG_49(__VA_ARGS__), __VA_ARGS__

#define D_INTR_MULT_ARG_N(N, ...) D_INTR_CALL_MULT_ARG_N(D_INTR_CONCAT(D_INTR_MULT_ARG_, N), (__VA_ARGS__))

// multiply arguments N times, puts comma in the end if N > 0
#define D_INTR_MULT_ARGC_0(...)
#define D_INTR_MULT_ARGC_1(...) __VA_ARGS__,
#define D_INTR_MULT_ARGC_2(...) __VA_ARGS__, D_INTR_MULT_ARGC_1(__VA_ARGS__)
#define D_INTR_MULT_ARGC_3(...) __VA_ARGS__, D_INTR_MULT_ARGC_2(__VA_ARGS__)
#define D_INTR_MULT_ARGC_4(...) __VA_ARGS__, D_INTR_MULT_ARGC_3(__VA_ARGS__)
#define D_INTR_MULT_ARGC_5(...) __VA_ARGS__, D_INTR_MULT_ARGC_4(__VA_ARGS__)
#define D_INTR_MULT_ARGC_6(...) __VA_ARGS__, D_INTR_MULT_ARGC_5(__VA_ARGS__)
#define D_INTR_MULT_ARGC_7(...) __VA_ARGS__, D_INTR_MULT_ARGC_6(__VA_ARGS__)
#define D_INTR_MULT_ARGC_8(...) __VA_ARGS__, D_INTR_MULT_ARGC_7(__VA_ARGS__)
#define D_INTR_MULT_ARGC_9(...) __VA_ARGS__, D_INTR_MULT_ARGC_8(__VA_ARGS__)
#define D_INTR_MULT_ARGC_10(...) __VA_ARGS__, D_INTR_MULT_ARGC_9(__VA_ARGS__)
#define D_INTR_MULT_ARGC_11(...) __VA_ARGS__, D_INTR_MULT_ARGC_10(__VA_ARGS__)
#define D_INTR_MULT_ARGC_12(...) __VA_ARGS__, D_INTR_MULT_ARGC_11(__VA_ARGS__)
#define D_INTR_MULT_ARGC_13(...) __VA_ARGS__, D_INTR_MULT_ARGC_12(__VA_ARGS__)
#define D_INTR_MULT_ARGC_14(...) __VA_ARGS__, D_INTR_MULT_ARGC_13(__VA_ARGS__)
#define D_INTR_MULT_ARGC_15(...) __VA_ARGS__, D_INTR_MULT_ARGC_14(__VA_ARGS__)
#define D_INTR_MULT_ARGC_16(...) __VA_ARGS__, D_INTR_MULT_ARGC_15(__VA_ARGS__)
#define D_INTR_MULT_ARGC_17(...) __VA_ARGS__, D_INTR_MULT_ARGC_16(__VA_ARGS__)
#define D_INTR_MULT_ARGC_18(...) __VA_ARGS__, D_INTR_MULT_ARGC_17(__VA_ARGS__)
#define D_INTR_MULT_ARGC_19(...) __VA_ARGS__, D_INTR_MULT_ARGC_18(__VA_ARGS__)
#define D_INTR_MULT_ARGC_20(...) __VA_ARGS__, D_INTR_MULT_ARGC_19(__VA_ARGS__)
#define D_INTR_MULT_ARGC_21(...) __VA_ARGS__, D_INTR_MULT_ARGC_20(__VA_ARGS__)
#define D_INTR_MULT_ARGC_22(...) __VA_ARGS__, D_INTR_MULT_ARGC_21(__VA_ARGS__)
#define D_INTR_MULT_ARGC_23(...) __VA_ARGS__, D_INTR_MULT_ARGC_22(__VA_ARGS__)
#define D_INTR_MULT_ARGC_24(...) __VA_ARGS__, D_INTR_MULT_ARGC_23(__VA_ARGS__)
#define D_INTR_MULT_ARGC_25(...) __VA_ARGS__, D_INTR_MULT_ARGC_24(__VA_ARGS__)
#define D_INTR_MULT_ARGC_26(...) __VA_ARGS__, D_INTR_MULT_ARGC_25(__VA_ARGS__)
#define D_INTR_MULT_ARGC_27(...) __VA_ARGS__, D_INTR_MULT_ARGC_26(__VA_ARGS__)
#define D_INTR_MULT_ARGC_28(...) __VA_ARGS__, D_INTR_MULT_ARGC_27(__VA_ARGS__)
#define D_INTR_MULT_ARGC_29(...) __VA_ARGS__, D_INTR_MULT_ARGC_28(__VA_ARGS__)
#define D_INTR_MULT_ARGC_30(...) __VA_ARGS__, D_INTR_MULT_ARGC_29(__VA_ARGS__)
#define D_INTR_MULT_ARGC_31(...) __VA_ARGS__, D_INTR_MULT_ARGC_30(__VA_ARGS__)
#define D_INTR_MULT_ARGC_32(...) __VA_ARGS__, D_INTR_MULT_ARGC_31(__VA_ARGS__)
#define D_INTR_MULT_ARGC_33(...) __VA_ARGS__, D_INTR_MULT_ARGC_32(__VA_ARGS__)
#define D_INTR_MULT_ARGC_34(...) __VA_ARGS__, D_INTR_MULT_ARGC_33(__VA_ARGS__)
#define D_INTR_MULT_ARGC_35(...) __VA_ARGS__, D_INTR_MULT_ARGC_34(__VA_ARGS__)
#define D_INTR_MULT_ARGC_36(...) __VA_ARGS__, D_INTR_MULT_ARGC_35(__VA_ARGS__)
#define D_INTR_MULT_ARGC_37(...) __VA_ARGS__, D_INTR_MULT_ARGC_36(__VA_ARGS__)
#define D_INTR_MULT_ARGC_38(...) __VA_ARGS__, D_INTR_MULT_ARGC_37(__VA_ARGS__)
#define D_INTR_MULT_ARGC_39(...) __VA_ARGS__, D_INTR_MULT_ARGC_38(__VA_ARGS__)
#define D_INTR_MULT_ARGC_40(...) __VA_ARGS__, D_INTR_MULT_ARGC_39(__VA_ARGS__)
#define D_INTR_MULT_ARGC_41(...) __VA_ARGS__, D_INTR_MULT_ARGC_40(__VA_ARGS__)
#define D_INTR_MULT_ARGC_42(...) __VA_ARGS__, D_INTR_MULT_ARGC_41(__VA_ARGS__)
#define D_INTR_MULT_ARGC_43(...) __VA_ARGS__, D_INTR_MULT_ARGC_42(__VA_ARGS__)
#define D_INTR_MULT_ARGC_44(...) __VA_ARGS__, D_INTR_MULT_ARGC_43(__VA_ARGS__)
#define D_INTR_MULT_ARGC_45(...) __VA_ARGS__, D_INTR_MULT_ARGC_44(__VA_ARGS__)
#define D_INTR_MULT_ARGC_46(...) __VA_ARGS__, D_INTR_MULT_ARGC_45(__VA_ARGS__)
#define D_INTR_MULT_ARGC_47(...) __VA_ARGS__, D_INTR_MULT_ARGC_46(__VA_ARGS__)
#define D_INTR_MULT_ARGC_48(...) __VA_ARGS__, D_INTR_MULT_ARGC_47(__VA_ARGS__)
#define D_INTR_MULT_ARGC_49(...) __VA_ARGS__, D_INTR_MULT_ARGC_48(__VA_ARGS__)
#define D_INTR_MULT_ARGC_50(...) __VA_ARGS__, D_INTR_MULT_ARGC_49(__VA_ARGS__)

#define D_INTR_MULT_ARGC_N(N, ...) D_INTR_CALL_MULT_ARGC_N(D_INTR_CONCAT(D_INTR_MULT_ARGC_, N), (__VA_ARGS__))

// returns 50 - N
#define D_INTR_50_MN(N) D_INTR_TAKE53(D_INTR_DUMMY_ARG, \
    D_INTR_EXP2 D_INTR_MULT_ARG_N(N, ()), \
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, \
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, \
    28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, \
    45, 46, 47, 48, 49, 50)

// returns N - 1, N > 0 && N <= 50
#define D_INTR_PREV(N) D_INTR_TAKE53(D_INTR_EXP2 D_INTR_MULT_ARG_N(N, ()), \
    50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
    32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, \
    14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

// returns N + 1, N >= 0 && N <= 50
#define D_INTR_NEXT(N) D_INTR_TAKE53(D_INTR_DUMMY_ARG, D_INTR_EXP2 D_INTR_MULT_ARG_N(N, ()), \
    51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
    32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, \
    14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)


#define D_INTR_GET_ARG_N_(N, ...) D_INTR_TAKE53(D_INTR_DUMMY_ARG, D_INTR_EXP2 D_INTR_MULT_ARG_N(D_INTR_50_MN(N), ()), \
    __VA_ARGS__, D_INTR_MULT_ARG_50(D_INTR_NO_SUCH_ARG))
#define D_INTR_GET_ARG_N(...) D_INTR_CALL_GETN(D_INTR_GET_ARG_N_, (__VA_ARGS__, D_INTR_NO_SUCH_ARG))


#define D_INTR_LSELF(...) __VA_ARGS__
#define LAST_D_INTR_LSELF(a, ...) a

#define D_INTR_EXCLUDE_LAST_L(...) D_INTR_CALL_W_I(D_INTR_MULT_ARG_R_N, \
    (D_INTR_CALL_W(D_INTR_PREV, (D_INTR_GET_ARGS_COUNT_L(__VA_ARGS__))), D_INTR_LSELF, D_INTR_GET_FIRST, D_INTR_SKIP_1E, __VA_ARGS__))

#define D_INTR_GET_ARG_DEF_1(...) __VA_ARGS__
#define D_INTR_GET_ARG_DEF_2(...) D_INTR_CALL_DEF(D_INTR_SKIP_1E, (__VA_ARGS__))
#define D_INTR_GET_ARG_DEF_L(...) D_INTR_CONCAT_W(D_INTR_GET_ARG_DEF_, D_INTR_GET_ARGS_COUNT_FEW2L(__VA_ARGS__))(__VA_ARGS__)
#define D_INTR_GET_ARG_DEF_M(...) D_INTR_CONCAT_W(D_INTR_GET_ARG_DEF_, D_INTR_GET_ARGS_COUNT_FEW2M(__VA_ARGS__))(__VA_ARGS__)

#define D_INTR_GET_ARG_DEF2_1(...) __VA_ARGS__
#define D_INTR_GET_ARG_DEF2_2(def1, ...) D_INTR_CALL_DEF2(D_INTR_SKIP_1E, (__VA_ARGS__))
#define D_INTR_GET_ARG_DEF2_L(def1, ...) D_INTR_CONCAT_W(D_INTR_GET_ARG_DEF2_, D_INTR_GET_ARGS_COUNT_FEW2L(__VA_ARGS__))(def1, __VA_ARGS__)
#define D_INTR_GET_ARG_DEF2_M(def1, ...) D_INTR_CONCAT_W(D_INTR_GET_ARG_DEF2_, D_INTR_GET_ARGS_COUNT_FEW2M(__VA_ARGS__))(def1, __VA_ARGS__)

#define D_INTR_FCALL_IF_ARGS_1(...)
#define D_INTR_FCALL_IF_ARGS_2(...) D_INTR_CALL_ARGS(D_INTR_GET_FIRST(__VA_ARGS__), (D_INTR_SKIP_1E(__VA_ARGS__))) // amount of arguments > 0
#define D_INTR_FCALL_IF_ARGS_L(...) D_INTR_CONCAT_W(D_INTR_FCALL_IF_ARGS_, D_INTR_GET_ARGS_COUNT_FEW2L(__VA_ARGS__))(__VA_ARGS__)
#define D_INTR_FCALL_IF_ARGS_M(...) D_INTR_CONCAT_W(D_INTR_FCALL_IF_ARGS_, D_INTR_GET_ARGS_COUNT_FEW2M(__VA_ARGS__))(__VA_ARGS__)

// compares A and B, A >= 0 && A <= 50, B >= 0 && B <= 50
// if A < B, returns L
// if A == B, returns E
// if A > B, returns G
#define D_INTR_CMP(A, B) D_INTR_GET_ARG_N(A, D_INTR_MULT_ARGC_N(B, L) E, D_INTR_MULT_ARG_50(G))

#define D_INTR_FCALL_IF_ARGS_GT_N_L_L(...) D_INTR_CALL_ARGS_GT(D_INTR_GET_FIRST(__VA_ARGS__), (D_INTR_SKIP_1E(__VA_ARGS__))) // amount of arguments > N >= 0
#define D_INTR_FCALL_IF_ARGS_GT_N_L_E(...)
#define D_INTR_FCALL_IF_ARGS_GT_N_L_G(...)
#define D_INTR_FCALL_IF_ARGS_GT_N_L(N, ...) D_INTR_CONCAT_W(D_INTR_FCALL_IF_ARGS_GT_N_L_, D_INTR_CMP(N, D_INTR_PREV(D_INTR_GET_ARGS_COUNT_L(__VA_ARGS__))))(__VA_ARGS__)

#define D_INTR_FREE_1_1(...) __VA_ARGS__
#define D_INTR_FREE_1_2(...) D_INTR_SELF __VA_ARGS__
#define D_INTR_FREE_1(...) D_INTR_CONCAT_W(D_INTR_FREE_1_, D_INTR_CALL_FREE(D_INTR_TAKE3, (D_INTR_EXP2 __VA_ARGS__, 2, 1)))(__VA_ARGS__)
#define D_INTR_FREE_2(...) __VA_ARGS__
#define D_INTR_FREE_L(...) D_INTR_CONCAT_W(D_INTR_FREE_, D_INTR_GET_ARGS_COUNT_FEW2L(__VA_ARGS__))(__VA_ARGS__)
#define D_INTR_FREE_M(...) D_INTR_CONCAT_W(D_INTR_FREE_, D_INTR_GET_ARGS_COUNT_FEW2M(__VA_ARGS__))(__VA_ARGS__)

#define D_INTR_REVERSE_PUT(arg1, r_seq) (D_INTR_SELF r_seq, arg1)
#define LAST_D_INTR_REVERSE_PUT(arg) (arg)
#define D_INTR_REVERSE_L(...) D_INTR_CALL_W(D_INTR_SELF, D_INTR_MULT_ARG_R_N(D_INTR_GET_ARGS_COUNT_L(__VA_ARGS__), \
    D_INTR_REVERSE_PUT, D_INTR_GET_FIRST, D_INTR_SKIP_1E, __VA_ARGS__))

#define D_INTR_SEQ_10_APPEND0(num) num ## 0
#define D_INTR_SEQ_10_POW(N) D_INTR_MULT_ARG_R_N(N, D_INTR_SELF, D_INTR_SELF, D_INTR_SEQ_10_APPEND0, 1)
