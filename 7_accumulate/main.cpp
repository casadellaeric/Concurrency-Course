#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>
#include <chrono>

template<typename iterator, typename T>
void custom_accumulate_seq(iterator begin, iterator end, T& initialValue)
{
    initialValue = std::accumulate(begin, end, 0);
}

template<typename iterator, typename T>
T custom_accumulate(iterator begin, iterator end, T initialValue)
{
    // If the block is smaller, it's not worth the overhead
    constexpr size_t MIN_BLOCK_SIZE{ 1000 };
    auto inputSize{ std::distance(begin, end) };
    auto numBlocks{ inputSize / (MIN_BLOCK_SIZE) };
    numBlocks = std::min(static_cast<unsigned int>(numBlocks), std::thread::hardware_concurrency());
    if (numBlocks == 0u) {
        numBlocks = 1u;
    }
    auto blockSize = inputSize / numBlocks;

    std::vector<T> values(numBlocks, {});

    // One of the blocks will be calculated by the current thread
    std::vector<std::thread> threads(numBlocks - 1);

    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i] = std::thread(custom_accumulate_seq<iterator, T>,
                                 begin + i * blockSize,
                                 begin + (i + 1) * blockSize,
                                 std::ref(values[i]));
    }
    values[numBlocks - 1]
        = std::accumulate(begin + (numBlocks - 1) * blockSize, begin + numBlocks * blockSize, 0);

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return std::accumulate(values.begin(), values.end(), initialValue);
}

int main()
{
    std::vector<unsigned int> input(1000000000u);
    std::iota(input.begin(), input.end(), 0);

    auto test = std::thread::hardware_concurrency();

    auto start = std::chrono::steady_clock::now();
    unsigned int stdAccumulate{ std::accumulate(input.begin(), input.end(), 0u) };
    auto end = std::chrono::steady_clock::now();
    std::cout << "The sum of the values in the input vector is: " << stdAccumulate
              << ". It took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              << "\n";

    start = std::chrono::steady_clock::now();
    unsigned int customAccumulate{ custom_accumulate(input.begin(), input.end(), 0u) };
    end = std::chrono::steady_clock::now();
    std::cout << "The sum of the values in the input vector is: " << customAccumulate
              << ". It took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
              << "\n";
}
