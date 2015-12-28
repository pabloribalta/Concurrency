/*
* Session 2, example 01:
*
* In the following example there is a global std::list and the functions add() and contains() operate on it.
*
* The access to the global std::list is regulated by a global std::mutex object thatprovides sequencial access to ensure
* that the invariant is not broken while reading while performing a write.
*
*/
#include <list>
#include <mutex>
#include <thread>
#include <iostream>

/* Scoped thread class from Session 1*/
class ThreadGuard
{
  std::thread t;

public:
  explicit ThreadGuard(std::thread& t_)
    : t(std::move(t_))
  {
    if (!t.joinable()) {
      throw std::logic_error("No thread");
    }
  }

  ~ThreadGuard()
  {
    t.join();
  }

  ThreadGuard(ThreadGuard const&) = delete;
  ThreadGuard& operator=(ThreadGuard const&) = delete;
};

std::list<int> myList; /* (!) Data structure which is not thread safe */
std::mutex myMutex; /* (!) Protects myList instance */

void add()
{
  for (int i = 0; i < 5; ++i) {
    std::lock_guard<std::mutex> guard(myMutex); /* (!) Lock the instance until adding is completed */
    myList.emplace_back(i);
  }
}

void contains(int i)
{
  std::lock_guard<std::mutex> guard(myMutex); /* (!) Lock the instance until finding is completed */

  if (std::find(myList.begin(), myList.end(), i) != myList.end()) {
    std::cout << i << " is contained in the list" << std::endl;
  } else {
    std::cout << i << " is NOT contained in the list" << std::endl;
  }
}

int main()
{
  ThreadGuard t1(std::thread (add));
  ThreadGuard t2(std::thread(contains, 0));

  return 0;
}

