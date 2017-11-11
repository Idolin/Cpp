#pragma once

#include "../expression/arithmetic_expr.h"

double *
fdm(Expression fa, Expression fb, Expression fc, Expression fd, double lb, double rb, double lvalue, double rvalue,
    unsigned points);
