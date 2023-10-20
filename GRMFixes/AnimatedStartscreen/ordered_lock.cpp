#include "ordered_lock.h"

ordered_lock::ordered_lock() : locked(false) {};

void ordered_lock::lock() {
    std::unique_lock<std::mutex> acquire(cvar_lock);
    if (locked) {
        std::condition_variable signal;
        cvar.emplace(&signal);
        signal.wait(acquire);
        cvar.pop();
    }
    else {
        locked = true;
    }
}

void ordered_lock::unlock() {
    std::unique_lock<std::mutex> acquire(cvar_lock);
    if (cvar.empty()) {
        locked = false;
    }
    else {
        cvar.front()->notify_one();
    }
}