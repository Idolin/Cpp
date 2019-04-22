//
// Created by cynder on 25.11.18.
//

#pragma once

#include "abstract_mapper.h"

// only one main thread, no parallelism
struct just_main: thread_mapper
{
    just_main();

    virtual void run(std::function<void()>) override;
    virtual void run_now(std::function<void()>) override;
    virtual void wait() override;
    virtual tid run_id(std::function<void()>) override;
    virtual tid run_now_id(std::function<void()>) override;
    virtual void wait(tid) override;
    virtual unsigned free_threads() override;
    virtual bool free() override;
};