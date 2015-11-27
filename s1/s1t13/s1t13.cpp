#include <thread>
#include <iostream>

struct BackgroundTask
{
  explicit BackgroundTask(const int /*state*/)
  {
  }

  void operator()() const
  {
    doSomething();
  }

private:
  static void doSomething()
  {
    std::cout << "Doing something in separate thread" << std::endl;
  }
};

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

void dummyFunction()
{
  ThreadGuard t1(std::thread(BackgroundTask(42)));

  std::cout << "Doing something in current thread" << std::endl;
}

int main()
{
  dummyFunction();

  return 0;
}

