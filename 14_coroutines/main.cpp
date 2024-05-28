#include <iostream>
#include <coroutine>
#include <cassert>

class Resumable
{
public:

    struct promise_type;

    Resumable(std::coroutine_handle<promise_type> handle) :
      m_handle(handle)
    {
        assert(handle);
    }
    Resumable(Resumable&)  = delete;
    Resumable(Resumable&&) = delete;
    ~Resumable() { m_handle.destroy(); }

    bool resume()
    {
        if (!m_handle.done()) {
            m_handle.resume();
        }
        return !m_handle.done();
    }

private:

    std::coroutine_handle<promise_type> m_handle;
};

struct Resumable::promise_type {
    auto get_return_object() { return std::coroutine_handle<promise_type>::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() { }
    void unhandled_exception() { std::terminate(); }
};

Resumable foo()
{
    std::cout << "First call.\n";
    co_await std::suspend_always();
    std::cout << "Second call.\n";
    co_await std::suspend_always();
    std::cout << "Third call.\n";
}

int main()
{
    Resumable resumable{ foo() };
    resumable.resume();
    resumable.resume();
}