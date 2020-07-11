#ifndef SPIN_MUTEX_H
#define SPIN_MUTEX_H
#include <atomic>

class SpinMutex {
private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    SpinMutex() = default;
    void lock() {
        while(flag.test_and_set(std::memory_order_acquire))
        ;
    }
    void unlock() {
        flag.clear(std::memory_order_release);
    }
};
#endif