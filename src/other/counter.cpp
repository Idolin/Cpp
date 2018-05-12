#include "counter.h"

time_counter::time_counter() = default;

void time_counter::start()
{
    time = std::chrono::steady_clock::now();
    saved = 0;
}

void time_counter::cont()
{
    time = std::chrono::steady_clock::now();
}

void time_counter::stop()
{
    saved += static_cast<unsigned long long>(
            std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::steady_clock::now() - time).count());
}

unsigned long long time_counter::getMilliseconds() const
{
    return saved / 1000;
}

unsigned long long time_counter::getMicroseconds() const
{
    return saved;
}

process_time_counter::process_time_counter() = default;

void process_time_counter::start()
{
#ifdef _WIN32
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    ASSERT(GetProcessTimes(GetCurrentProcess(),
        &createTime, &exitTime, &kernelTime, &time) != -1);
    milliseconds = 0;
#else
    ASSERT(getrusage(RUSAGE_SELF, &time) != -1);
    microseconds = 0;
#endif
}

void process_time_counter::cont()
{
#ifdef _WIN32
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    ASSERT(GetProcessTimes(GetCurrentProcess(),
        &createTime, &exitTime, &kernelTime, &time) != -1);
#else
    ASSERT(getrusage(RUSAGE_SELF, &time) != -1);
#endif
}

void process_time_counter::stop()
{
#ifdef _WIN32
    FILETIME createTime;
    FILETIME exitTime;
    FILETIME kernelTime;
    FILETIME userTime;
    ASSERT(GetProcessTimes(GetCurrentProcess(),
        &createTime, &exitTime, &kernelTime, &userTime) != -1);
    SYSTEMTIME userSystemTime;
    SYSTEMTIME userSystemTimeNow;
    ASSERT(FileTimeToSystemTime(&userTime, &userSystemTime) != -1);
    ASSERT(FileTimeToSystemTime(&userTime, &userSystemTimeNow) != -1);
    milliseconds += static_cast<unsigned long long>(
            userSystemTimeNow.wHour - userSystemTime.wHour) * 3600000 +
        static_cast<unsigned long long>(
            userSystemTimeNow.wMinute - userSystemTime.wMinute) * 60000 +
        static_cast<unsigned long long>(
            userSystemTimeNow.wSecond - userSystemTime.wSecond) * 1000 +
        static_cast<unsigned long long>(
            userSystemTimeNow.wMilliseconds - userSystemTime.wMilliseconds);
#else
    rusage now;
    ASSERT(getrusage(RUSAGE_SELF, &now) != -1);
    microseconds += static_cast<unsigned long long>(now.ru_utime.tv_sec -
                                                    time.ru_utime.tv_sec) * 1000000 +
            static_cast<unsigned long long>(now.ru_utime.tv_usec - time.ru_utime.tv_usec);
#endif
}

unsigned long long process_time_counter::getMilliseconds() const
{
#ifdef _WIN32
    return milliseconds;
#else
    return microseconds / 1000;
#endif
}

unsigned long long process_time_counter::getMicroseconds() const
{
#ifdef _WIN32
    return milliseconds * 1000;
#else
    return microseconds;
#endif
}

clocks_counter::clocks_counter() = default;

void clocks_counter::start()
{
    clocks = std::clock();
}

void clocks_counter::cont()
{
    clocks += std::clock();
}

void clocks_counter::stop()
{
    clocks = std::clock() - clocks;
}

unsigned long long clocks_counter::getMilliseconds() const
{
    return (static_cast<unsigned long long>(clocks) / CLOCKS_PER_SEC) * 1000 +
            ((static_cast<unsigned long long>(clocks) % CLOCKS_PER_SEC) * 1000) / CLOCKS_PER_SEC;
}

unsigned long long clocks_counter::getMicroseconds() const
{
    return (static_cast<unsigned long long>(clocks) / CLOCKS_PER_SEC) * 1000000 +
           ((static_cast<unsigned long long>(clocks) % CLOCKS_PER_SEC) * 1000000) / CLOCKS_PER_SEC;
}