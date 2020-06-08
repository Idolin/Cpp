#include "turing_machine.h"

#include "../template/arraymethods.hpp"

turing_machine_exception::turing_machine_exception(const char *s): message(s)
{}

const char *turing_machine_exception::what() const noexcept
{
    return message;
}


turing_machine::tape::tape(): cells(new cell[100]), from(-50), to(50),
                              current(0), state_names(), blank("blank")
{
    state_names.insert(blank, 0);
}

turing_machine::tape::tape(str *states, unsigned len): cells(new cell[len]),
                                                       from(0), to(len), current(0),
                                                       state_names(), blank("blank")
{
    state_names.insert(blank, 0);
    unsigned long long c = 0;
    for(unsigned i = 0;i < len;i++)
    {
        checkIdentifier(states[i]);
        auto it = state_names.findKey(states[i]);
        if(!(it))
        {
            cells[c++].state = static_cast<unsigned int>(state_names.size());
            state_names.insert(states[i], static_cast<unsigned int>(state_names.size()));
        }
        else
            cells[c++].state = *it;
    }
}

turing_machine::tape::tape(str states): from(0), current(0),
                                        state_names(), blank("blank")
{
    str del = "|";
    to = states.count(del) + 1;
    cells = new cell[to];
    state_names.insert(blank, 0);
    unsigned long p = 0;
    unsigned c = 0;
    for(unsigned long t = states.find(del);t != str::not_found;t = states.find(del, p))
    {
        str s = states(p, t);
        p = t + 1;
        checkIdentifier(s);
        auto it = state_names.findKey(s);
        if(!it)
        {
            cells[c++].state = static_cast<unsigned int>(state_names.size());
            state_names.insert(s, static_cast<unsigned int>(state_names.size()));
        }
        else
            cells[c++].state = *it;
    }
    str s = states(p);
    checkIdentifier(s);
    auto it = state_names.findKey(s);
    if(!it)
    {
        cells[c++].state = static_cast<unsigned int>(state_names.size());
        state_names.insert(s, static_cast<unsigned int>(state_names.size()));
    }
    else
        cells[c++].state = *it;
}

turing_machine::tape::tape(str states, bool ignore)
{
    for(unsigned long i = 0;i < states.length();i++)
    {
        str s = states(i, i + 1);
        checkIdentifier(s);
        auto it = state_names.findKey(s);
        if(!it)
        {
            cells[i].state = static_cast<unsigned int>(state_names.size());
            state_names.insert(s, static_cast<unsigned int>(state_names.size()));
        }
        else
            cells[i].state = *it;
    }
}

turing_machine::tape::~tape()
{
    delete [] cells;
}

turing_machine::tape& turing_machine::tape::operator=(tape&& otr) noexcept
{
    delete [] cells;
    cells = otr.cells;
    from = otr.from;
    to = otr.to;
    current = otr.current;
    state_names = std::move(otr.state_names);
    blank = std::move(otr.blank);
    otr.cells = nullptr;
    otr.from = 0;
    otr.to = 0;
    otr.current = 0;
    return *this;
}

void turing_machine::tape::clear()
{
    cell blank_cell = cell();
    blank_cell.state = 0;
    _vfill(cells, to - from, blank_cell);
    current = 0;
}

str turing_machine::tape::getById(unsigned id) const
{
    for(auto it = state_names.begin();it != state_names.end();it++)
        if(*it == id)
            return it.key();
    ASSERT(false, "tape: state with id: %u not exists", id);
    return "";
}

void turing_machine::tape::step(std::pair<moves, unsigned> actions)
{
    cells[current - from].state = actions.second;
    if(actions.first != moves::KEEP)
    {
        if(actions.first == moves::LEFT)
        {
            if(--current < from)
                double_size(false);
        }
        else
        {
            if(++current >= to)
                double_size();
        }
    }
}

void turing_machine::tape::double_size(bool to_right)
{
    if(to_right)
    {
        cells = _resize(cells, to - from, (to - from) * 2);
        to += to - from;
    }
    else
    {
        cell *tmp = new cell[(to - from) * 2];
        _copy(tmp + (to - from), to - from, cells);
        delete [] cells;
        cells = tmp;
        from -= to - from;  
    }
}

void turing_machine::tape::display()
{
    long long blank_left_end = from, blank_right_end = to;
    unsigned long long offset = 0;
    for(;blank_left_end < current - 1;blank_left_end++)
        if(cells[blank_left_end].state)
            break;
        else
            offset++;
    offset *= blank.length() + 1;
    for(;blank_right_end > current + 1;blank_right_end--)
        if(cells[blank_right_end].state)
            break;
    for(long long i = blank_left_end;i < current;i++)
        ++offset += getById(cells[i].state).length();
    printf("%s%s\n", static_cast<const char*>(str(" ") * offset),
           static_cast<const char*>(str("v") * getById(cells[current].state).length()));
    for(long long i = _max(blank_left_end - 1, from);i < _min(blank_right_end, to - 1);i++)
        printf("%s|", getById(cells[i].state).c_str());
    printf("%s\n", getById(cells[_min(blank_right_end, to - 1)].state).c_str());
}

turing_machine::finite_auto::finite_auto(): states(3), state_names(), start("start"),
                                            accepted("accept"), rejected("reject")
{
    state_names.insert(accepted, 0);
    state_names.insert(rejected, 1);
    state_names.insert(start, 2);
    states[2].transitions.empty();
    current = &states[2];
}

turing_machine::finite_auto::~finite_auto() = default;

str turing_machine::finite_auto::getById(unsigned id)
{
    for(auto it = state_names.begin();it != state_names.end();it++)
        if(*it == id)
            return it.key();
    ASSERT(false, "automate: state with id: %u not exists!", id);
    return "";
}

str turing_machine::finite_auto::getByPointer(state *s)
{
    unsigned stateID = 0;
    for(state& sf: states)
        if(&sf != s)
            stateID++;
        else
            break;
    return getById(stateID);
}

std::pair<turing_machine::moves, unsigned> turing_machine::finite_auto::step(unsigned cell_state)
{
    auto it = current->transitions.findKey(cell_state);
    if(!it && (cell_state & turing_machine::tape::special))
    {
        if(cell_state & 1)//turing_machine::tape::tilded)
            it = current->transitions.findKey(
                    turing_machine::tape::special | 1); //turing_machine::tape::tilded);
        else
            it = current->transitions.findKey(turing_machine::tape::special);
    }
    if(!it)
        it = current->transitions.findKey(static_cast<unsigned>(-1));
    if(it)
    {
        current = std::get<2>(*it);
        unsigned new_cell_state = std::get<1>(*it);
        if(new_cell_state == static_cast<unsigned>(-1))
            new_cell_state = cell_state;
        return std::make_pair(std::get<0>(*it), new_cell_state);
    }
    current = &states[1];
    return std::make_pair(moves::KEEP, cell_state);
}

void turing_machine::finite_auto::display(str state_name, unsigned cell_state)
{
    auto it = current->transitions.findKey(cell_state);
    printf("%s->[%s]->%s\n", getByPointer(current).c_str(), state_name.c_str(),
           (it) ? getByPointer(std::get<2>(*it)).c_str() : rejected.c_str());
}

turing_machine::turing_machine(): t(), f(), step(0)
{}

turing_machine::~turing_machine() = default;

void turing_machine::reset()
{
    step = 0;
    t.clear();
    f.current = &f.states[2];
}

void turing_machine::setBlank(str name)
{
    if(step)
        throw turing_machine_exception("Can't change state after machine started");
    checkIdentifier(name);
    if(t.state_names.findKey(name) && name != t.blank)
        throw turing_machine_exception(name + " already exists as non-blank state");
    t.state_names.erase(t.state_names.findKey(t.blank));
    t.blank = name;
    t.state_names.insert(name, 0);
}

void turing_machine::setStartState(str name)
{
    if(step)
        throw turing_machine_exception("Can't change state after machine started");
    checkIdentifier(name);
    if(f.state_names.findKey(name) && name != f.start)
        throw turing_machine_exception(name + " already exists as non-start state");
    f.state_names.erase(t.state_names.findKey(f.start));
    f.start = name;
    f.state_names.insert(name, 2);
}

void turing_machine::setAcceptedState(str name)
{
    if(step)
        throw turing_machine_exception("Can't change state after machine started");
    checkIdentifier(name);
    if(f.state_names.findKey(name) && name != f.accepted)
        throw turing_machine_exception(name + " already exists not as accepted state");
    f.state_names.erase(t.state_names.findKey(f.accepted));
    f.accepted = name;
    f.state_names.insert(name, 0);
}

void turing_machine::setRejectedState(str name)
{
    if(step)
        throw turing_machine_exception("Can't change state after machine started");
    checkIdentifier(name);
    if(f.state_names.findKey(name) && name != f.rejected)
        throw turing_machine_exception(name + " already exists not as rejected state");
    f.state_names.erase(f.state_names.findKey(f.rejected));
    f.rejected = name;
    f.state_names.insert(name, 1);
}

void turing_machine::transition(str fa_state, str cell_state,
                                turing_machine::moves move, str new_state, str new_cell_state)
{
    if(!f.state_names.findKey(fa_state))
        throw turing_machine_exception("automate state not exists: " + fa_state);
    if(cell_state != "*" && cell_state != "_" && cell_state != "~" &&
       !t.state_names.findKey(cell_state))
        throw turing_machine_exception("tape state not exists: " + cell_state);
    if(new_state == "")
        new_state = fa_state;
    if(new_cell_state == "")
        new_cell_state = (cell_state == "_" || cell_state == "~") ? "*" : cell_state;
    if(!f.state_names.findKey(new_state))
    {
        checkIdentifier(new_state);
        f.state_names.insert(new_state, static_cast<unsigned int>(f.state_names.size()));
        f.states[f.states.size()].transitions.empty();
    }
    if(new_cell_state != "*" && !t.state_names.findKey(new_cell_state))
    {
        checkIdentifier(new_cell_state);
        t.state_names.insert(new_cell_state, static_cast<unsigned int>(t.state_names.size()));
    }
    unsigned cellID;
    if(cell_state == "*")
        cellID = static_cast<unsigned>(-1);
    elif(cell_state == "_")
        cellID = tape::special;
    elif(cell_state == "~")
        cellID = tape::special | tape::tilda;
    else
        cellID = *t.state_names.findKey(cell_state);
    f.states[*f.state_names.findKey(fa_state)].transitions.insert(
            cellID,
            std::make_tuple(move,
                            (new_cell_state == "*") ?
                            static_cast<unsigned>(-1) :
                            *t.state_names.findKey(new_cell_state),
                    &f.states[*f.state_names.findKey(new_state)]));
}

void turing_machine::setTape(str *states, unsigned len)
{
    reset();
    t = tape(states, len);
}

void turing_machine::setTape(str states)
{
    reset();
    t = tape(states);
}

void turing_machine::setTapeChars(str states)
{
    reset();
    t = tape(states, true);
}

unsigned long long turing_machine::compute(unsigned long long max_steps)
{
    unsigned long long steps = 0;
    while(!isTerminate() && steps < max_steps)
    {
        step++;
        steps++;
        take_step();
    }
    return steps;
}

str turing_machine::getCurrentState()
{
    return f.getByPointer(f.current);
}

str turing_machine::getCurrentCell()
{
    return t.getById(t.cells[t.current].state);
}

bool turing_machine::isTerminate()
{
    return isAccepted() || isRejected();
}

bool turing_machine::isAccepted()
{
    return f.current == &f.states[0];
}

bool turing_machine::isRejected()
{
    return f.current == &f.states[1];
}

void turing_machine::take_step()
{
    t.step(f.step(t.cells[t.current].state));
}

void turing_machine::displayState()
{
    f.display(t.getById(t.cells[t.current].state), t.cells[t.current].state);
    t.display();
}

void turing_machine::displayFiniteAuto()
{
    for(unsigned id : f.state_names)
    {
        finite_auto::state *s = &f.states[id];
        for(auto it = s->transitions.begin();it != s->transitions.end();it++)
            printf("%s %s %s %s %s\n", f.getById(id).c_str(), t.getById(it.key()).c_str(),
                   to_string(std::get<0>(*it)), f.getByPointer(std::get<2>(*it)).c_str(),
                   t.getById(std::get<1>(*it)).c_str());
    }
}

void turing_machine::display()
{
    displayFiniteAuto();
    displayState();
}

bool turing_machine::checkIdentifier(str name)
{
    ASSERT(name.length() > 0);
    if(name == "*" || name == "~")
        return true;
    for(char c: name)
        ASSERT(('0' <= c) and (c <= '9') or ('a' <= c) and (c <= 'z') or
               ('A' <= c) and (c <= 'Z') or (c == '_'));
    return true;
}

str turing_machine::to_string(turing_machine::moves move)
{
    switch(move)
    {
        case LEFT: return "<";
        case KEEP: return "=";
        default: return ">";
    }
}

void turing_algo::get_left(str to)
{
    str next_state = get_next();
    for(str s : to.split("|"))
        if(s == "")
            m.transition(last_state, m.t.blank, turing_machine::KEEP, next_state);
        else
            m.transition(last_state, s, turing_machine::KEEP, next_state);
    m.transition(last_state, '*', turing_machine::moves::LEFT, last_state);
    last_state = next_state;
}

void turing_algo::get_right(str to)
{
    str next_state = get_next();
    for(str s : to.split("|"))
        if(s == "")
            m.transition(last_state, m.t.blank, turing_machine::KEEP, next_state);
        else
            m.transition(last_state, s, turing_machine::KEEP, next_state);
    m.transition(last_state, '*', turing_machine::moves::RIGHT, last_state);
    last_state = next_state;
}

void turing_algo::clear_left()
{
    str next_state = get_next();
    m.transition(last_state, m.t.blank, turing_machine::KEEP, next_state);
    m.transition(last_state, "*", turing_machine::LEFT, last_state, m.t.blank);
    last_state = next_state;
}

void turing_algo::clear_right()
{
    str next_state = get_next();
    m.transition(last_state, m.t.blank, turing_machine::KEEP, next_state);
    m.transition(last_state, "*", turing_machine::RIGHT, last_state, m.t.blank);
    last_state = next_state;
}

void turing_algo::set_(str tag)
{
    ASSERT(tag == "" || tag == "_" || tag == "~");
    str next_state = get_next();
    m.transition(last_state, "*", turing_machine::KEEP, next_state,
                 (tag == "") ? " " : tag);
    last_state = next_state;
}

void turing_algo::label(str name)
{
    turing_machine::checkIdentifier(name);
    ASSERT(!(label_to_state.findKey(name)), "Label name already exists: %s", name.c_str());
    label_to_state.insert(name, last_state);
}

void turing_algo::goto_label(str name)
{
    auto it = label_to_state.findKey(name);
    ASSERT(it, "Label not found: %s", name.c_str());
    m.transition(last_state, "*", turing_machine::KEEP, *it);
    last_state = get_next();
}

str turing_algo::get_next()
{
    static unsigned id = 0;
    return str("s") + str(id++);
}

void turing_algo::set_cell(str state)
{
    str next_state = get_next();
    m.transition(last_state, "*", turing_machine::KEEP, next_state,
                 state);
    last_state = next_state;
}

void turing_algo::left()
{
    str next_state = get_next();
    m.transition(last_state, "*", turing_machine::LEFT, next_state);
    last_state = next_state;
}

void turing_algo::right()
{
    str next_state = get_next();
    m.transition(last_state, "*", turing_machine::RIGHT, next_state);
    last_state = next_state;
}

void turing_algo::add()
{
    str next_state = get_next();
    read();

}

void turing_algo::if_cell(str cell)
{
    if_states.push(last_state);
//    for(str s : cell.split("|"))
//        add_transitions(s);
//    state_next();
}

void turing_algo::if_var(str cell, bool index)
{
    if_states.push(last_state);
    if(index)
        for(str s : cell.split("|"))
        {
            str check = "~" + s;
//            for(str state : get_states())
//                if(state.endswith(check))
//                    add_transition(state);
        }
    else
        for(str s : cell.split("|"))
        {
            str check = "~" + s + "~";
//            for(str state : get_states())
//                if(state.startswith(check))
//                    add_transition(state);
        }
//    state_next();
}

void turing_algo::end_if()
{
//    ASSERT(!if_states.empty());
//    add_transitions("*", last_state = if_states.pop());
}


