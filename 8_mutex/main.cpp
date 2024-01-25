#include <iostream>
#include <thread>
#include <mutex>
#include <stack>

template<typename T>
class SafeStack
{
public:

    SafeStack(){};

    void push(T val)
    {
        std::lock_guard<std::mutex> lg(m);
        m_s.push(std::make_shared<T>(std::move(val)));
    };

    T pop()
    {
        std::lock_guard<std::mutex> lg(m);
        if (m_s.empty()) {
            throw std::runtime_error("Cannot call pop() on an empty stack!");
        }
        T topVal{ *m_s.top().get() };
        m_s.pop();
        return topVal;
    };

    void pop(T& val)
    {
        std::lock_guard<std::mutex> lg(m);
        if (m_s.empty()) {
            throw std::runtime_error("Cannot call pop() on an empty stack!");
        }
        val = *m_s.top().get();
        m_s.pop();
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lg(m);
        return m_s.size();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lg(m);
        return m_s.empty();
    }

private:

    std::stack<std::shared_ptr<T>> m_s{};
    std::mutex m{};
};

int main()
{
    SafeStack<int> s{};
    for (size_t i = 0; i < 10; ++i) {
        s.push(static_cast<int>(i));
    }

    auto size{ s.size() };
    for (size_t i = 0; i < size; ++i) {
        std::cout << s.pop() << std::endl;
    }
}