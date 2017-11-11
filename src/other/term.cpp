#include "term.h"

void set_term_color(char color, FILE *ostream)
{
    fprintf(ostream, "\x1b[%dm", color);
}
