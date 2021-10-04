//
// Created by cynder on 25.11.18.
//

#include "just_main.h"

just_main::just_main()
{}

void just_main::run(std::function<void()> task)
{
    task();
}

void just_main::run_now(std::function<void()> task)
{
    task();
}

void just_main::wait()
{}

tid just_main::run_id(std::function<void()> task)
{
    task();
    return 0;
}

tid just_main::run_now_id(std::function<void()> task)
{
    task();
    return 0;
}

void just_main::wait(tid)
{}

unsigned just_main::free_threads()
{
    return 0;
}

bool just_main::free()
{
    return true;
}
