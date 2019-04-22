//
// Created by cynder on 20.04.19.
//

#pragma once

#include <atomic>
#include <thread>
#include <type_traits>

#include "../debug/def_debug.h"
#include "flag_wait_algo.hpp"

template<unsigned wait = 100>
struct spinlock
{
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    ~spinlock()
    {
        ASSERT_NO_THROW(!flag.test_and_set(std::memory_order_acquire));
    }

    void lock()
    {
        wait_for_flag<wait>(flag);
    }

    bool try_lock()
    {
        return !flag.test_and_set(std::memory_order_acquire);
    }

    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};