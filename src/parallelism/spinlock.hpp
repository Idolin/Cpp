//
// Created by cynder on 20.04.19.
//

#pragma once

#include <atomic>
#include <thread>
#include <type_traits>

#include "../debug/def_debug.h"

namespace __internal
{

    template<bool enable = true>
    struct recursive
    {
        thread_local static unsigned nesting_level;
    };

    template<bool enable> thread_local unsigned recursive<enable>::nesting_level = 0;
}

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
struct spinlock<true, false, false>: protected __internal::recursive<true>
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
        if(nesting_level++ == 0)
            while(flag.test_and_set(std::memory_order_acquire))
                std::this_thread::yield();
    }

    bool try_lock()
    {
        if(nesting_level > 0 || !flag.test_and_set(std::memory_order_acquire))
        {
            nesting_level++;
            return true;
        }
        return false;
    }

    void unlock()
    {
        if(--nesting_level == 0)
            flag.clear(std::memory_order_release);
    }
};

template<>
struct spinlock<false, true, false>
{
private:
    std::atomic<bool> r_flag = {true}; //read allow
    std::atomic<unsigned> r_counter = {0};

public:
    ~spinlock()
    {
        ASSERT_NO_THROW(r_flag.load() && r_counter.load() == 0);
    }

    void lock()
    {
        while(!r_flag.exchange(false, std::memory_order_acquire)) //wait for previous write_unlock
            std::this_thread::yield();
        while(r_counter.load(std::memory_order_acquire)) //wait for all read_unlocks
            std::this_thread::yield();
    }

    void rLock()
    {
        while(!r_flag.exchange(false, std::memory_order_acquire)) //wait for previous write_unlock
            std::this_thread::yield();
        r_counter++;
        r_flag.store(true, std::memory_order_acq_rel);
    }

    bool try_lock()
    {
        if(r_flag.exchange(false, std::memory_order_acquire))
        {
            if(r_counter.load(std::memory_order_acquire) == 0)
                return true;
            r_flag.store(true, std::memory_order_acq_rel);
        }
        return false;
    }

    bool try_rLock()
    {
        if(r_flag.exchange(false, std::memory_order_acquire))
        {
            r_counter++;
            r_flag.store(true, std::memory_order_acq_rel);
            return true;
        }
        return false;
    }

    void shift_to_rLock()
    {
        r_counter++;
        r_flag.store(true, std::memory_order_release);
    }

    void unlock()
    {
        if(r_counter.load(std::memory_order_release))
            r_counter--;
        else
            r_flag.store(true);
    }
};

template<> //calling lock after rLock is UB
struct spinlock<true, true, false>: __internal::recursive<true>
{
private:
    std::atomic<bool> r_flag = {true}; //read allow
    std::atomic<unsigned> r_counter = {0};

public:
    ~spinlock()
    {
        ASSERT_NO_THROW(r_flag.load() && r_counter.load() == 0);
    }

    void lock()
    {
        if(nesting_level++ == 0)
        {
            while(!r_flag.exchange(false, std::memory_order_acquire)) //wait for previous write_unlock
                std::this_thread::yield();
            while(r_counter.load(std::memory_order_acquire)) //wait for all read_unlocks
                std::this_thread::yield();
        }
    }

    void rLock()
    {
        if(nesting_level++ == 0)
        {
            while(!r_flag.exchange(false, std::memory_order_acquire)) //wait for previous write_unlock
                std::this_thread::yield();
            r_counter++;
            r_flag.store(true, std::memory_order_acq_rel);
        }
    }

    bool try_lock()
    {
        if(nesting_level > 0)
            return true;
        if(r_flag.exchange(false, std::memory_order_acquire))
        {
            if(r_counter.load(std::memory_order_acquire) == 0)
            {
                nesting_level++;
                return true;
            }
            r_flag.store(true, std::memory_order_acq_rel);
        }
        return false;
    }

    bool try_rLock()
    {
        if(nesting_level > 0)
            return true;
        if(r_flag.exchange(false, std::memory_order_acquire))
        {
            r_counter++;
            r_flag.store(true, std::memory_order_acq_rel);
            nesting_level++;
            return true;
        }
        return false;
    }

    void shift_to_rLock()
    {
        r_counter++;
        r_flag.store(true, std::memory_order_release);
    }

    void unlock()
    {
        if(--nesting_level == 0)
        {
            if(r_counter.load(std::memory_order_release))
                r_counter--;
            else
                r_flag.store(true);
        }
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
    thread_local static q_node node;

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
};

thread_local typename spinlock<false, false, true>::q_node spinlock<false, false, true>::node;
