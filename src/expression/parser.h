#pragma once

#include "arithmetic_expr.h"

#include <stdio.h>
#include <string>

#define elif else if

using namespace std;

extern const char *operations;
extern const char *spaces;

struct parseException
{
    const char *msg;
    unsigned position;

    parseException(string msg, unsigned position);

    void message();
};

Expression parse(string s);
