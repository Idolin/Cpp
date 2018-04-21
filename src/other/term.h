#pragma once

#include "../other/defdef.h"

#include <stdio.h>

enum term_color
{
    RED = 31,
    GREEN = 32,
    ORANGE = 33,
    BLUE = 34,
    DEFAULT = 39
};

void set_term_color(char color = term_color::DEFAULT, FILE *ostream = stdout);

#define color_printf(color, ...) \
{ \
    set_term_color(color); \
    printf(__VA_ARGS__); \
    set_term_color(); \
}

#define color_fprintf(color, ostream, ...) \
{ \
    set_term_color(color, ostream); \
    fprintf(ostream, __VA_ARGS__); \
    set_term_color(term_color::DEFAULT, ostream); \
}

inline void color_fputs(term_color color, const char *str, FILE *ostream = stdout)
{
    set_term_color(color, ostream);
    fputs(str, ostream);
    set_term_color(term_color::DEFAULT, ostream);
}
