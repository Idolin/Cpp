//
// Created by cynder on 25.11.18.
//

#pragma once

#include <functional>

using tid = unsigned; // task id

// Model:
// to_run - queue of functions to be run
// every run call adds a task to to_run queue
// after, when there is a free thread it pops a task and runs it
struct thread_mapper
{
    // add task to to_run queue
    virtual void run(std::function<void()> task) = 0;

    // run task immidiately in separate thread if possible, otherwise in main thread
    virtual void run_now(std::function<void()> task) = 0;

    // wait for all tasks in to_run queue to finish
    virtual void wait() = 0;

    // like run, but returns task id
    virtual tid run_id(std::function<void()> task) = 0;

    // like run_now, but returns task id
    virtual tid run_now_id(std::function<void()> task) = 0;

    // wait for task with given id to finish
    virtual void wait(tid id) = 0;

    // returns amount of tasks which definitely can be run in background for now
    virtual unsigned free_threads() = 0;

    // returns true if there is no tasks run in background
    virtual bool free() = 0;
};