//
// Created by cynder on 25.11.18.
//

#pragma once

#include "pthread.h

#include "abstract_mapper.h"

// n threads using pthread
struct pthread_n_threads: thread_mapper
{
    pid_t *threads;

    pthread_n_threads();

    virtual void run(std::function<void()>) override;
    virtual void run_now(std::function<void()>) override;
    virtual void wait() override;
    virtual tid run_id(std::function<void()>) override;
    virtual tid run_now_id(std::function<void()>) override;
    virtual void wait(tid) override;
    virtual unsigned free_threads() override;
    virtual bool free() override;
};