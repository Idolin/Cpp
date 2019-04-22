//
// Created by cynder on 21.04.19.
//

#pragma once

namespace wait
{
    static const unsigned forever = std::numeric_limits<unsigned>::max();
}

template<unsigned wait>
inline typename std::enable_if_t<wait == wait::forever> wait_for_flag(std::atomic_flag& flag)
{
    while(flag.test_and_set(std::memory_order_acquire));
}

template<unsigned wait>
inline typename std::enable_if_t<wait == 0> wait_for_flag(std::atomic_flag& flag)
{
    while(flag.test_and_set(std::memory_order_acquire))
        std::this_thread::yield();
}

template<unsigned wait>
inline typename std::enable_if_t<wait != 0 && wait != wait::forever> wait_for_flag(std::atomic_flag& flag)
{
    unsigned instructions_left = wait;
    while(instructions_left--)
        if(!flag.test_and_set(std::memory_order_acquire))
            return;
    while(flag.test_and_set(std::memory_order_acquire))
        std::this_thread::yield();
}