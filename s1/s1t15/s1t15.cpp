#include <algorithm>
#include <numeric>
#include <thread>
#include <vector>
#include <random>
#include <iostream>

template<typename Iterator, typename T>
struct accumulateBlock
{
  void operator()(Iterator first, Iterator last, T& result)
  {
    result = std::accumulate(first, last, result);
  }
};

template<typename Iterator, typename T>
T accumulateParallel(Iterator first, Iterator last, T init)
{
  auto length = std::distance(first, last);

  if (!length) {
    return init;
  }

  const unsigned long min_per_thread = 25;
  const unsigned long max_threads = (length + min_per_thread - 1) / min_per_thread;
  const unsigned long hardware_threads = std::thread::hardware_concurrency();
  const unsigned int num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

  const auto block_size = length / num_threads;

  std::vector<T> results(num_threads);
  std::vector<std::thread> threads(num_threads - 1);

  Iterator block_start = first;

  for (unsigned long i = 0; i < (num_threads - 1); ++i) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    threads[i] = std::thread(accumulateBlock<Iterator, T>(), block_start, block_end, std::ref(results[i]));
    block_start = block_end;
  }

  accumulateBlock<Iterator, T>()(block_start, last, results[num_threads - 1]);
  std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

  return std::accumulate(results.begin(), results.end(), init);
}

template<typename T>
static std::vector<T> generate(T size)
{
  static std::uniform_int_distribution<T> distribution(0, 10);

  static std::default_random_engine generator;

  std::vector<T> data(size);
  std::generate(data.begin(), data.end(), []() {
                  return distribution(generator);
                });
  return data;
}

int main()
{
  auto numbers(generate(10));

  std::cout << "Numbers to add:";

  for (auto&& number : numbers) {
    std::cout << number << " ";
  }

  std::cout << std::endl << "Result: " << accumulateParallel(numbers.begin(), numbers.end(), 0) << std::endl;

  return 0;
}

