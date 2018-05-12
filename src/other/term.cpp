#include "term.h"

void set_term_color(char color, FILE *ostream)
{
    if(color != term_color::KEEP)
        fprintf(ostream, "\x1b[%dm", color);
}
