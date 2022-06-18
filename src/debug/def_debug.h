#pragma once

#define DEBUG

#define DEBUG_LVL 5
#define DEBUG_OUTPUT_STREAM stderr

#include "../other/defdef.h"
#include "../other/term.h"

#include <cstdio>
#include <exception>

/*
 * DEBUG_LVL levels description
 * 0: critical errors
 * 1: error
 * 2: exception
 * 3: not-critical error
 * >: some helpful debug messages
*/

class assert_failed: public std::exception
{
public:
    explicit assert_failed(const char *s, const char *file = nullptr, unsigned long line = 0, int err_code = -1);

    ~assert_failed();

    const char *what() const noexcept override;

    int err_code() const noexcept;

private:
    mutable const char *message;
    int error_code;
    bool allocated;
};


#ifdef DEBUG

    #ifndef DEBUG_LVL
        #define DEBUG_LVL 5
    #endif

    #define DEBUGLVLMSG_N(level, ...) \
        { \
            if(level <= DEBUG_LVL) \
            { \
                if(level <= 1) \
                    fprintf(stderr, __VA_ARGS__); \
                else \
                    fprintf(DEBUG_OUTPUT_STREAM, __VA_ARGS__); \
            } \
        }
    #define DEBUGLVLMSG(level, ...) \
        { \
            DEBUGLVLMSG_N(level, __VA_ARGS__); \
            if(level <= DEBUG_LVL) \
            { \
                if(level <= 1) \
                    fputc('\n', stderr); \
                else \
                    fputc('\n', DEBUG_OUTPUT_STREAM); \
            } \
        }
    #define DEBUGMSG(...) DEBUGLVLMSG(5, __VA_ARGS__)
    #define DEBUGLVLIFMSG(level, condition, ...) \
        { \
            if(condition) \
                DEBUGLVLMSG(level, __VA_ARGS__); \
        }
    #define DEBUGIFMSG(condition, ...) DEBUGLVLIFMSG(4, condition, __VA_ARGS__)
    // ASSERT_NO_THROW(condition[, printf args...])
    #define ASSERT_NO_THROW(...) \
        { \
            if(!(GET_FIRST(__VA_ARGS__))) \
            { \
                set_term_color(term_color::RED, stderr); \
                DEBUGLVLMSG(1, "%s %d in file %s!", "Assertion failed at line", __LINE__, __FILE__); \
                set_term_color(term_color::DEFAULT, stderr); \
                FCALL_IF_ARGS_GT_N_L(1, DEBUGLVLMSG, 1 CSKIP_1L(__VA_ARGS__)); \
                fflush(DEBUG_OUTPUT_STREAM); \
            } \
        }
    // ASSERT_ERR(condition, error_code[, printf args...])
    #define ASSERT_ERR(condition, ...) \
        { \
            if(!(condition)) \
            { \
                ASSERT_NO_THROW(false CSKIP_1L(__VA_ARGS__)); \
                throw assert_failed("Assertion failed!", __FILE__, __LINE__, (GET_FIRST(__VA_ARGS__))); \
            } \
        }
    // ASSERT(condition[, printf args...])
    #define ASSERT(...) ASSERT_ERR((GET_FIRST(__VA_ARGS__)), -1 CSKIP_1L(__VA_ARGS__))
    // ASSERT_CODE_ZERO(expression[, printf args...]) -> invokes ASSERT if expression returns non-zero code
    #define ASSERT_CODE_ZERO(...) ASSERT((GET_FIRST(__VA_ARGS__)) == 0 CSKIP_1L(__VA_ARGS__))

#else

    #define DEBUGLVLMSG_N(level, ...) {}
    #define DEBUGLVLMSG(level, ...) {}
    #define DEBUGMSG(...) {}
    #define DEBUGLVLIFMSG(level, cond, ...) {}
    #define DEBUGIFMSG(cond, ...) {}
    #define ASSERT_NO_THROW(...) {}
    #define ASSERT_ERR(condition, ...) {}
    #define ASSERT(...) {}
    #define ASSERT_CODE_ZERO(...) GET_FIRST(__VA_ARGS__);

#endif

