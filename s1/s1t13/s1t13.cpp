/*
* Session 1, example 13
*
* One  benefit  of  the  move  support  of std::thread is  that  you  can  build  on  the ThreadGuard
* class from example s01t06 and have it actually take ownership of the thread.
*
* This avoids any unpleasant consequences should the ThreadGuard object outlive the thread  it  was  
* referencing,  and  it  also  means  that  no  one  else  can  join  or  detach the thread  once  
* ownership  has  been  transferred  into  the  object.  
*
* The implementation is shown, along with a simple example.
*
*/
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
    : t(std::move(t_)) /* (!) Acquire ownership of the supplied thread */
  {
    if (!t.joinable()) { /* (!) In this example we choose to verify if the thread is joinable in the constructor */
      throw std::logic_error("No thread");
    }
  }

  ~ThreadGuard()
  {
    t.join(); /* (!) When the obejct gets out of scope or destroyed, wait for the thread to join */
  }

  ThreadGuard(ThreadGuard const&) = delete;
  ThreadGuard& operator=(ThreadGuard const&) = delete;
};

void dummyFunction()
{
  ThreadGuard t1(std::thread(BackgroundTask(42)));

  std::cout << "Doing something in current thread" << std::endl;
} /* (!) ThreadGuard is destroyed and joined with the thread supplied to the constructor */

int main()
{
  dummyFunction();

  return 0;
}

