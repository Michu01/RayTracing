#pragma once

#include <chrono>
#include <thread>
#include <condition_variable>

template<typename _Rep, typename _Period>
void SleepFor(const std::chrono::duration<_Rep, _Period>& dur, const std::stop_token& stoken)
{
    std::condition_variable_any cv;
    std::mutex mutex_;
    std::unique_lock<std::mutex> ul_{ mutex_ };
    std::stop_callback stop_wait{ stoken, [&cv]() { cv.notify_one(); } };
    cv.wait_for(ul_, dur, [&stoken]() { return stoken.stop_requested(); });
}