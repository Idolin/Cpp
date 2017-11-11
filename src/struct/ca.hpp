#pragma once

#include "../other/commonmethods.hxx"
#include "../tree/tree.hxx"
#include "../container/vector.hxx"

#include <algorithm>
#include <stdarg.h>

using std::pair;
using std::make_pair;

#define BLACK 0
#define RED 1
#define GREEN 2
#define ORANGE 3
#define BLUE 4
#define PURPLE 5
#define LIGHT_BLUE 6
#define GRAY 7
#define PREVIOUS 8
#define DEFAULT 9

void changecolor(unsigned char fg = 9, unsigned char bg = 9)
{
    fprintf(stdout, "\033[3%dm\033[4%dm", fg, bg);
}

void cursorMove(unsigned short x = 0, unsigned short y = 0)
{
    fprintf(stdout, "\033[%u;%uH", y, x);
}

#define ANY 0xfffe
#define BORDER 0xffff

bool warn(bool f, const char *format, ...)
{
    if(f)
    {
        fputs("Warn:", stderr);
        va_list args;
        va_start(args, format);
        fprintf(stderr, format, args);
        va_end(args);
    };
    return f;
}

void warn(const char *format, ...)
{
    fputs("Warn:", stderr);
    va_list args;
    va_start(args, format);
    fprintf(stderr, format, args);
    va_end(args);
}

struct cchar
{
    unsigned char color;
    char c;

    cchar() : color(DEFAULT * 16 + DEFAULT), c('0')
    {}

    cchar(char c, unsigned char bg = DEFAULT, unsigned char fg = DEFAULT) : color((unsigned char) (bg * 16 + fg)), c(c)
    {}

    void print()
    {
        changecolor(color & 15, (color & 240) >> 4);
        putchar(c);
    }
};

struct automat
{
    struct state
    {
        cchar r;
        unsigned short n;

        unsigned short (*turn)(unsigned short left, unsigned short state, unsigned short right);

        state() : r(cchar()), n(0), turn(0)
        {}

        state(cchar r, unsigned short n,
              unsigned short(turn)(unsigned short left, unsigned short state, unsigned short right) = 0) :
                r(r), n(n), turn(turn)
        {}
    };

    unsigned turnN, len;
    unsigned short *field;
    bool cycled, stable;
    vect <state> *states;
    vect <avl_tree<unsigned short, pair<unsigned short, unsigned short>>> *rules;

    unsigned short (*turnDef)(unsigned short left, unsigned short state, unsigned short right);

    automat(unsigned len, bool cycled = true,
            unsigned short(turn)(unsigned short, unsigned short, unsigned short) = 0) :
            turnN(0), len(len), field(new unsigned short[len]), cycled(cycled), stable(turnDef),
            states(new vect <state>), rules(new vect <avl_tree<unsigned short, pair<unsigned short, unsigned short>>>),
            turnDef(turn)
    {
        _fill(field, len);
        this->setState();
    }

    ~automat()
    {
        delete[] field;
        delete rules;
    }

    void setState(cchar r = cchar(),
                  unsigned short(turn)(unsigned short left, unsigned short state, unsigned short right) = 0,
                  unsigned short stateID = (unsigned short) -1)
    {
        if(stateID == (unsigned short) -1)
            stateID = (unsigned short) states->maxs;
        else
            stable = false;
        states->set(stateID, state(r, stateID, turn));
        if(rules->size < stateID)
            rules->resizeUp();
    }

    void putState(unsigned position, unsigned short state)
    {
        if(warn(position >= len, "Position not exist"))
            position %= len;
        field[position] = state;
        stable = false;
    }

    void setRule(unsigned short l, unsigned short m, unsigned short r, unsigned short result, bool rewrite = true)
    {
        if(l == ANY)
            for(unsigned short i = BORDER; (i < states->maxs) || (i == BORDER); i++)
                setRule(i, m, r, result, rewrite);
        elif(m == ANY)
        for(unsigned short i = 0; i < states->maxs; i++)
            setRule(l, i, r, result, rewrite);
        elif(r == ANY)
        for(unsigned short i = BORDER; (i < states->maxs) || (i == BORDER); i++)
            setRule(l, m, i, result, rewrite);
        else
        rules->m[m].insert(make_pair(l, r), result, rewrite);
    }

    unsigned short turnDefault(unsigned short l, unsigned short m, unsigned short r)
    {
        unsigned short ret = *(rules->m[m].findKey(make_pair(l, r)));
        if(!ret)
            ret = m;
        return ret;
    }

    unsigned short getNewState(unsigned short left, unsigned short middle, unsigned short right)
    {
        unsigned short res = BORDER;
        if(states->get(middle).turn)
            res = states->get(middle).turn(left, middle, right);
        if(res == BORDER && turnDef)
            res = turnDef(left, middle, right);
        if(res == BORDER)
            res = turnDefault(left, middle, right);
        if(warn(res >= states->maxs, "Returned state not exists"))
            res %= (unsigned short) states->maxs;
        return res;
    }

    void show(unsigned short line = 0, unsigned short col = 0)
    {
        cursorMove((unsigned short) (col + 1), (unsigned short) (line + 1));
        for(unsigned i = 0; i < len; i++)
            states->get(field[i]).r.print();
        changecolor();
    }

    void turn()
    {
        if(stable)
            return;
        stable = true;
        if(warn(len == 0, "Field have zero length"))
            return;
        unsigned short sfirst = field[0], sprev, stmp;
        sfirst = field[0];
        sprev = cycled ? field[len - 1] : BORDER;
        for(unsigned i = 0; i < len - 1; i++)
        {
            stmp = field[i];
            field[i] = getNewState(sprev, field[i], field[i + 1]);
            stable = stable && (field[i] == stmp);
            sprev = stmp;
        }
        field[len - 1] = getNewState(sprev, field[len - 1], cycled ? sfirst : BORDER);
        if(!stable)
            turnN++;
    }
};
