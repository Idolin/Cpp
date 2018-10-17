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
 * 0: critical errors, should never happen, different crashes can follow
 * 1: non-critical, exception or data corruption, possible crashes
 * 2: definitely bug, can lead to exception, no crashes will follow
 * 3: some not-critical bugs for program works, program will continue working without problem
 * >: some helpful debug messages
*/

class assert_failed: public std::exception
{
public:
    explicit assert_failed(const char *s, int err_code = -1);

    const char *what() const noexcept override;

    int err_code() const noexcept;

private:
    const char *message;
    int error_code;
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
#define ASSERT_ERR(cond, err_code, ...) \
    { \
        if(!(cond)) \
        { \
            set_term_color(term_color::RED, stderr); \
            DEBUGLVLMSG(1, "%s %d in file %s!", "Assertion failed at line", __LINE__, __FILE__); \
            set_term_color(term_color::DEFAULT, stderr); \
            START_IF_ARGS_NDEF(DEBUGLVLMSG, 1, 1, ## __VA_ARGS__); \
            throw assert_failed("Assertion failed!", err_code); \
        } \
    }
#define ASSERT(cond, ...) ASSERT_ERR(cond, -1, ## __VA_ARGS__)

#else
#define DEBUGLVLMSG_N(...) {}
#define DEBUGLVLMSG(...) {}
#define DEBUGMSG(...) {}
#define DEBUGLVLIFMSG(...) {}
#define DEBUGIFMSG(...) {}
#define ASSERT_ERR(...) {}
#define ASSERT(...) {}
#endif

