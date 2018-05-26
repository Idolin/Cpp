#pragma once

struct turing_machine
{
    union moves
    {
        LEFT;
        KEEP;
        RIGHT;
    };

    struct cell
    {
        unsigned state;
    };

    struct tape
    {
        cell *cells;
        long long from, to, current;
        avl<str, unsigned> state_names;
    };

    struct finite_auto
    {
        struct state
        {
            avl_tree<unsigned, state*> transitions;
            bool terminate;
        };

        vect<state*> states;
        avl<str, unsigned> state_names;
    };

    turing_machine();

    ~turing_machine();

    void setBlank(str name);

    void transition(str cell_state, str fa_state, moves move = RIGHT, str new_state = '');

    void setTape(tape t);

    unsigned long long compute(unsigned long long max_steps = 10000000000);

    str getCurrentState();

    str getCurrentCell();

    bool isTerminate();

    void displayState();

    void displayFiniteAuto();

    void display();
};