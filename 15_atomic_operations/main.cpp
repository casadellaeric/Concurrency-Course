#include <atomic>
#include <mutex>

class spinlock_mutex
{
    spinlock_mutex() { }

    void lock()
    {
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }

    void unlock() { flag.clear(std::memory_order_release); }

private:

    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

spinlock_mutex m;

int main()
{
    // Same thing
    std::atomic<int> aInt{};
    std::atomic_int bInt{};

    // atomic_flag
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
    flag.test_and_set();
    flag.clear();

    // lock_guard with custom mutex

    std::lock_guard<spinlock_mutex> lg(m);
}