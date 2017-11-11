#pragma once

#include "../other/arraymethods.hpp"
#include "../container/vector.hpp"

struct fsm
{
    struct transition
    {
        unsigned state;
        char c;

        transition()
        {}

        transition(unsigned state, char c) : state(state), c(c)
        {}
    };

    bool *accept, dfa, *tmp;
    unsigned states, start;
    vect<transition> *transitions;

    fsm(unsigned statesAmount, bool deterministic = true);

    ~fsm();

    void addTransition(unsigned from, unsigned to, char c);

    inline bool _transition(unsigned &state, char c);

    bool checkAccept(const char *s, unsigned len);

    unsigned wordsCount(unsigned length = -1, unsigned mod = 1000000007);
};
