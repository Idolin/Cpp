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
                fprintf(stderr, ## __VA_ARGS__); \
            else \
                fprintf(DEBUG_OUTPUT_STREAM, ## __VA_ARGS__); \
        } \
    }
#define DEBUGLVLMSG(level, ...) \
    { \
        DEBUGLVLMSG_N(level, ## __VA_ARGS__); \
        if(level <= DEBUG_LVL) \
        { \
            if(level <= 1) \
                fputc('\n', stderr); \
            else \
                fputc('\n', DEBUG_OUTPUT_STREAM); \
        } \
    }
#define DEBUGMSG(...) DEBUGLVLMSG(5, ## __VA_ARGS__)
#define DEBUGLVLIFMSG(level, cond, ...) \
    { \
        if(cond) \
            DEBUGLVLMSG(level, ## __VA_ARGS__); \
    }
#define DEBUGIFMSG(cond, ...) DEBUGLVLIFMSG(4, cond, ## __VA_ARGS__)
#define ASSERT_NO_THROW(cond, ...) \
    { \
        if(!(cond)) \
        { \
            set_term_color(term_color::RED, stderr); \
            DEBUGLVLMSG(1, "%s %d in file %s!", "Assertion failed at line", __LINE__, __FILE__); \
            set_term_color(term_color::DEFAULT, stderr); \
            START_IF_ARGS_NDEF(DEBUGLVLMSG, 1, 1, ## __VA_ARGS__); \
            fflush(DEBUG_OUTPUT_STREAM); \
        } \
    }
#define ASSERT_ERR(cond, err_code, ...) \
    { \
        if(!(cond)) \
        { \
            ASSERT_NO_THROW(false, ## __VA_ARGS__); \
            throw assert_failed("Assertion failed!", __FILE__, __LINE__, (err_code)); \
        } \
    }
#define ASSERT(cond, ...) ASSERT_ERR((cond), -1, ## __VA_ARGS__)
#define ASSERT_CODE_NOT(func, fail_code, ...) ASSERT((func) != (fail_code), ## __VA_ARGS__)
#define ASSERT_CODE_OK(func, ...) ASSERT((func) == 0, ## __VA_ARGS__)

#else
#define DEBUGLVLMSG_N(level, ...) {}
#define DEBUGLVLMSG(level, ...) {}
#define DEBUGMSG(...) {}
#define DEBUGLVLIFMSG(level, cond, ...) {}
#define DEBUGIFMSG(cond, ...) {}
#define ASSERT_NO_THROW(cond, ...) {}
#define ASSERT_ERR(cond, err_code, ...) {}
#define ASSERT(cond, ...) {}
#define ASSERT_CODE_NOT(func, fail_code, ...) func;
#define ASSERT_CODE_OK(func, ...) func;
#endif

