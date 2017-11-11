#pragma once

#define elif else if
#define smin_(a, b) (a = ((a < b) ? (a) : (b)))
#define smax_(a, b) (a = ((a < b) ? (b) : (a)))

#define TAKE53(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
                 _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
                 _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, ...) _53
#define MULT_ARG_50M50(X)
#define MULT_ARG_50M49(X) X
#define MULT_ARG_50M48(X) X, X
#define MULT_ARG_50M47(X) X, MULT_ARG_50M48(X)
#define MULT_ARG_50M46(X) X, MULT_ARG_50M47(X)
#define MULT_ARG_50M45(X) X, MULT_ARG_50M46(X)
#define MULT_ARG_50M44(X) X, MULT_ARG_50M45(X)
#define MULT_ARG_50M43(X) X, MULT_ARG_50M44(X)
#define MULT_ARG_50M42(X) X, MULT_ARG_50M43(X)
#define MULT_ARG_50M41(X) X, MULT_ARG_50M42(X)
#define MULT_ARG_50M40(X) X, MULT_ARG_50M41(X)
#define MULT_ARG_50M39(X) X, MULT_ARG_50M40(X)
#define MULT_ARG_50M38(X) X, MULT_ARG_50M39(X)
#define MULT_ARG_50M37(X) X, MULT_ARG_50M38(X)
#define MULT_ARG_50M36(X) X, MULT_ARG_50M37(X)
#define MULT_ARG_50M35(X) X, MULT_ARG_50M36(X)
#define MULT_ARG_50M34(X) X, MULT_ARG_50M35(X)
#define MULT_ARG_50M33(X) X, MULT_ARG_50M34(X)
#define MULT_ARG_50M32(X) X, MULT_ARG_50M33(X)
#define MULT_ARG_50M31(X) X, MULT_ARG_50M32(X)
#define MULT_ARG_50M30(X) X, MULT_ARG_50M31(X)
#define MULT_ARG_50M29(X) X, MULT_ARG_50M30(X)
#define MULT_ARG_50M28(X) X, MULT_ARG_50M29(X)
#define MULT_ARG_50M27(X) X, MULT_ARG_50M28(X)
#define MULT_ARG_50M26(X) X, MULT_ARG_50M27(X)
#define MULT_ARG_50M25(X) X, MULT_ARG_50M26(X)
#define MULT_ARG_50M24(X) X, MULT_ARG_50M25(X)
#define MULT_ARG_50M23(X) X, MULT_ARG_50M24(X)
#define MULT_ARG_50M22(X) X, MULT_ARG_50M23(X)
#define MULT_ARG_50M21(X) X, MULT_ARG_50M22(X)
#define MULT_ARG_50M20(X) X, MULT_ARG_50M21(X)
#define MULT_ARG_50M19(X) X, MULT_ARG_50M20(X)
#define MULT_ARG_50M18(X) X, MULT_ARG_50M19(X)
#define MULT_ARG_50M17(X) X, MULT_ARG_50M18(X)
#define MULT_ARG_50M16(X) X, MULT_ARG_50M17(X)
#define MULT_ARG_50M15(X) X, MULT_ARG_50M16(X)
#define MULT_ARG_50M14(X) X, MULT_ARG_50M15(X)
#define MULT_ARG_50M13(X) X, MULT_ARG_50M14(X)
#define MULT_ARG_50M12(X) X, MULT_ARG_50M13(X)
#define MULT_ARG_50M11(X) X, MULT_ARG_50M12(X)
#define MULT_ARG_50M10(X) X, MULT_ARG_50M11(X)
#define MULT_ARG_50M9(X) X, MULT_ARG_50M10(X)
#define MULT_ARG_50M8(X) X, MULT_ARG_50M9(X)
#define MULT_ARG_50M7(X) X, MULT_ARG_50M8(X)
#define MULT_ARG_50M6(X) X, MULT_ARG_50M7(X)
#define MULT_ARG_50M5(X) X, MULT_ARG_50M6(X)
#define MULT_ARG_50M4(X) X, MULT_ARG_50M5(X)
#define MULT_ARG_50M3(X) X, MULT_ARG_50M4(X)
#define MULT_ARG_50M2(X) X, MULT_ARG_50M3(X)
#define MULT_ARG_50M1(X) X, MULT_ARG_50M2(X)
#define MULT_ARG_50M0(X) X, MULT_ARG_50M1(X)

#define MULT_ARG_50MN(N, ...) MULT_ARG_50M ## N(__VA_ARGS__)

#define GET_FIRST(_1, ...) _1
#define GET_SECOND(_1, _2, ...) _2
#define SELF(...) __VA_ARGS__

#define CONCAT_(a, b) a ## b
#define CONCAT(a, b) CONCAT_(a, b)

#define GET_ARG_N(N, ...) SELF(TAKE53 SELF() (2, 2, 2, MULT_ARG_50MN(N, _NO_SUCH_ARG_), ## __VA_ARGS__))
#define GET_ARGS_COUNT(...) TAKE53(53, 52, ## __VA_ARGS__, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, \
                                                24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define GET_ARG_LAST(...) GET_ARG_N(GET_ARGS_COUNT(__VA_ARGS__), __VA_ARGS__)
#define GET_ARGS_COUNT_3(...) SELF(TAKE53 SELF() (2, 2, 2, 2, ## __VA_ARGS__, MULT_ARG_50M3(2), 1, 0))
#define _GET_ARG_DEF__0() _NO_SUCH_ARG_
#define _GET_ARG_DEF__1(a) a
#define _GET_ARG_DEF__2(a, ...) __VA_ARGS__
#define GET_ARG_DEF(def, ...) CONCAT(_GET_ARG_DEF__, GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)
#define GET_ARG_DEF_(def, ...) CONCAT(_GET_ARG_DEF__, GET_ARGS_COUNT_3(def, ## __VA_ARGS__))(def, ## __VA_ARGS__)

#define _START_IF_ARGS__0(f)
#define _START_IF_ARGS__1(f, ...) f(__VA_ARGS__)
#define _START_IF_ARGS__2(f, ...) _START_IF_ARGS__1(f, ## __VA_ARGS__)
#define START_IF_ARGS(function, ...) CONCAT(_START_IF_ARGS__, GET_ARGS_COUNT_3(__VA_ARGS__))(function, ## __VA_ARGS__)

#define GET_ARGS_COUNT_N(N, ...) SELF(TAKE53 SELF() (2, ## __VA_ARGS__, MULT_ARG_50MN(N, 2), 1, MULT_ARG_50M3(0)))
#define _START_IF_ARGS_NDEF__0(...)
#define _START_IF_ARGS_NDEF__1(f, ...) f(__VA_ARGS__)
#define _START_IF_ARGS_NDEF__2(f, ...) _START_IF_ARGS_NDEF__1(f, ## __VA_ARGS__)
#define START_IF_ARGS_NDEF(function, N, ...) CONCAT(_START_IF_ARGS_NDEF__, GET_ARGS_COUNT_N(N, ## __VA_ARGS__))(function, ## __VA_ARGS__)