//
// Created by cynder on 20.04.19.
//

#pragma once

#include <atomic>
#include <thread>
#include <type_traits>

#include "../debug/def_debug.h"

template<bool recursive=false, bool read_lock=false, bool queued = false>
struct spinlock;

template<>
struct spinlock<false, false, false>
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
        while(flag.test_and_set(std::memory_order_acquire))
            std::this_thread::yield();
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

template<>
struct spinlock<false, false, true>
{
    struct q_node
    {
        std::atomic<q_node*> next;
        std::atomic_flag flag;
    };

private:
    std::atomic<q_node*> tail = {nullptr};

public:
    ~spinlock()
    {
        ASSERT_NO_THROW(tail == nullptr);
    }

    void lock()
    {
        node.next.store(nullptr, std::memory_order_acquire), node.flag.test_and_set(std::memory_order_acquire);
        q_node* tmp = &node;
        tmp = tail.exchange(tmp, std::memory_order_seq_cst);
        if(tmp != nullptr)
        {
            tmp->next.store(&node, std::memory_order_seq_cst);
            while(node.flag.test_and_set(std::memory_order_acquire))
                std::this_thread::yield();
        }
    }

    bool try_lock()
    {
        node.next.store(nullptr, std::memory_order_acquire), node.flag.test_and_set(std::memory_order_acquire);
        q_node* tmp = &node;
        return tail.compare_exchange_strong(tmp, nullptr, std::memory_order_seq_cst);
    }

    void unlock()
    {
        q_node* tmp = &node;
        if(!tail.compare_exchange_strong(tmp, nullptr, std::memory_order_acquire))
        {
            while(node.next.load(std::memory_order_acquire) == nullptr);
            node.next.load(std::memory_order_acquire)->flag.clear(std::memory_order_release);
        }
    }

    thread_local static q_node node;
};

thread_local typename spinlock<false, false, true>::q_node spinlock<false, false, true>::node;
