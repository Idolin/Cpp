#include "fsm.h"

fsm::fsm(unsigned statesAmount, bool deterministic) : accept(new bool[statesAmount]), dfa(deterministic),
                                                      tmp(new bool[statesAmount * 2]), states(statesAmount), start(0),
                                                      transitions(new vect<transition>[statesAmount])
{
    _fill(accept, states);
}

fsm::~fsm()
{
    delete[] transitions;
    delete[] accept;
    delete[] tmp;
}

void fsm::addTransition(unsigned from, unsigned to, char c)
{
    transitions[from].push(transition(to, c));
}

inline bool fsm::_transition(unsigned &state, char c)
{
    for(unsigned i = 0; i < transitions[state].maxs; i++)
        if(transitions[state][i].c == c)
        {
            state = transitions[state][i].state;
            return true;
        }
    return false;
}

bool fsm::checkAccept(const char *s, unsigned len)
{
    if(dfa)
    {
        unsigned state = start;
        for(unsigned i = 0; i < len; i++)
            if(not _transition(state, s[i]))
                return false;
        return accept[state];
    } else
    {
        _fill(tmp, states * 2);
        tmp[start] = true;
        unsigned a = 0;
        for(unsigned i = 0; i < len; i++)
        {
            for(unsigned j = 0; j < states; j++)
                if(tmp[j + a])
                    for(unsigned k = 0; k < transitions[j].maxs; k++)
                        if(transitions[j][k].c == s[i])
                            tmp[transitions[j][k].state + states - a] = true;
            _fill(tmp + a, states);
            a = states - a;
        }
        for(unsigned i = 0; i < states; i++)
            if((accept[i]) and (tmp[i + a]))
                return true;
        return false;
    }
}

unsigned fsm::wordsCount(unsigned length, unsigned mod)
{
    unsigned *u_tmp = new unsigned[states * 2];
    _fill(u_tmp, states * 2);
    u_tmp[start] = 1;
    unsigned len;
    if(length == (unsigned) -1)
    {
        len = states * 2;
        _fill(tmp, states);
    } else
        len = length;
    unsigned a = 0, sum = (accept[start]);
    for(unsigned i = 0; i < len; i++)
    {
        for(unsigned j = 0; j < states; j++)
            if(u_tmp[j + a])
                for(unsigned k = 0; k < transitions[j].maxs; k++)
                {
                    u_tmp[transitions[j][k].state + states - a] = (u_tmp[j + a] +
                                                                   u_tmp[transitions[j][k].state + states - a]) % mod;
                    if(accept[transitions[j][k].state])
                        sum = (sum + u_tmp[j + a]) % mod;
                    tmp[transitions[j][k].state] = (tmp[j] || accept[j]);
                }
        _fill(u_tmp + a, states);
        a = states - a;
    }
    delete[] u_tmp;
    if(length == (unsigned) -1)
        for(unsigned i = 0; i < states; i++)
            if(accept[i] && tmp[i])
                return -1;
    return sum;
}
