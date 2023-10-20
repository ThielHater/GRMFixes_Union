#pragma once

// https://stackoverflow.com/a/14792685

#include <mutex>
#include <condition_variable>
#include <queue>

class ordered_lock {
public:
    ordered_lock();
    void lock();
    void unlock();

private:
    std::queue<std::condition_variable*> cvar;
    std::mutex cvar_lock;
    bool locked;
};