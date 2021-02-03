#pragma once

#include "../string/cstr.h"
#include "../tree/avl.hpp"
#include "../container/vector.hpp"
#include "../debug/def_debug.h"
#include "../template/commonmethods.hpp"
#include "../container/stack.hpp"
#include <exception>

struct turing_machine_exception: std::exception
{
public:
    explicit turing_machine_exception(const char *s);

    const char *what() const noexcept override;

private:
    const char *message;
};

struct turing_machine
{
    enum moves
    {
        LEFT,
        KEEP,
        RIGHT
    };

    struct cell
    {
        unsigned state;
    };

    struct tape
    {
        cell *cells;
        long long from, to, current;
        avl_tree<unsigned, str> state_names;
        str blank;

        tape();
        tape(str *states, unsigned len);
        explicit tape(str states);
        tape(str states, bool ignore);
        ~tape();

        tape& operator=(tape&&) noexcept;

        void clear();
        str getById(unsigned) const;
        void step(std::pair<moves, unsigned> actions);
        void double_size(bool to_right=true);
        void display();

        static const unsigned special = (std::numeric_limits<unsigned>::max() >> 1u) + 1u;
        static const unsigned tilda = (std::numeric_limits<unsigned>::max() >> 2u) + 1u;
    };

    struct finite_auto
    {
        struct state
        {
            avl_tree<std::tuple<moves, unsigned, state*>, unsigned> transitions;
        };

        vect<state> states;
        avl_tree<unsigned, str> state_names;
        state *current;
        str start, accepted, rejected;

        finite_auto();
        ~finite_auto();

        str getById(unsigned);
        str getByPointer(state*);
        std::pair<moves, unsigned> step(unsigned cell_state);
        void display(str, unsigned);
    };

    tape t;
    finite_auto f;
    unsigned long long step;

    turing_machine();

    ~turing_machine();

    void reset();

    void setBlank(str name);

    void setStartState(str name);

    void setAcceptedState(str name);

    void setRejectedState(str name);

    void transition(str fa_state, str cell_state = "*", moves move = RIGHT,
                    str new_state = "", str new_cell_state = "");

    void setTape(str*, unsigned);

    void setTape(str);

    void setTapeChars(str);

    unsigned long long compute(unsigned long long max_steps = 10000000000);

    str getCurrentState();

    str getCurrentCell();

    bool isTerminate();

    bool isAccepted();

    bool isRejected();

    void displayState();

    void displayFiniteAuto();

    void display();

    static bool checkIdentifier(str name);

private:
    void take_step();

    static str to_string(moves move);
};

struct turing_algo
{
    turing_machine m;
    str last_state;
    avl_tree<str, str> label_to_state;
    vect<str> read_states[2];
    stack<str> if_states;

    turing_algo();
    ~turing_algo();

    void get_left(str to);
    void get_right(str to);
    void clear_left();
    void clear_right();
    void set_(str tag);
    void label(str name);
    void goto_label(str name);
    void set_cell(str state = "");
    void left();
    void right();
    void add();
    void if_cell(str cell);
    void if_var(str cell, bool index = 0);
    void end_if();
    void read(bool index = 0);
    void write(bool index = 0);
    void erase(bool index = 0);

private:
    str get_next();
};