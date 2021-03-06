#pragma once

#include <ctime>
#include <chrono>

#include "../debug/def_debug.h"

#ifdef _WIN32

#include <Windows.h>
#else

#include <sys/resource.h>
#include <sys/times.h>
#endif

struct counter
{
    virtual void start() = 0; // begin time measurement
    virtual void cont() = 0; // continue measure after stop
    virtual void stop() = 0; // stop/pause timer
    virtual void clear() = 0;
    virtual unsigned long long getMilliseconds() const = 0;
    virtual unsigned long long getMicroseconds() const;
    virtual unsigned long long getNanoseconds() const;
    virtual unsigned long long getPrecision() const = 0; // tick period in nanoseconds
};

struct time_counter: counter
{
private:
    std::chrono::steady_clock::time_point time;
    unsigned long long saved;
public:
    time_counter();
    void start() override;
    void cont() override;
    void stop() override;
    void clear() override;
    unsigned long long getMilliseconds() const override;
    unsigned long long getMicroseconds() const override;
    unsigned long long getNanoseconds() const override;
    unsigned long long getPrecision() const override;
};

struct process_time_counter: counter
{
private:
#ifdef _WIN32
    FILETIME time;
    unsigned long long milliseconds;
#else
    rusage time;
    unsigned long long microseconds;
#endif
public:
    process_time_counter();
    void start() override;
    void cont() override;
    void stop() override;
    void clear() override;
    unsigned long long getMilliseconds() const override;
    unsigned long long getMicroseconds() const override;
    unsigned long long getPrecision() const override;
};

struct clocks_counter: counter
{
private:
    std::clock_t clocks;
public:
    clocks_counter();
    void start() override;
    void cont() override;
    void stop() override;
    void clear() override;
    unsigned long long getMilliseconds() const override;
    unsigned long long getMicroseconds() const override;
    unsigned long long getNanoseconds() const override;
    unsigned long long getPrecision() const override;
    unsigned long long getClocks() const;
};
