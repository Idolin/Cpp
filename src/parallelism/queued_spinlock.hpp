//
// Created by cynder on 20.04.19.
//

#pragma once

#include <atomic>
#include <thread>
#include <type_traits>

#include "../debug/def_debug.h"
#include "../other/counter.h"

namespace
{
    struct q_node
    {
        std::atomic<q_node*> next;
        std::atomic_flag flag;
    };

    thread_local q_node node;
}

template<unsigned wait = 100>
struct q_spinlock
{
private:
    std::atomic<q_node*> tail = {nullptr};

public:
    ~q_spinlock()
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
            wait_for_flag<wait>(node.flag);
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
};