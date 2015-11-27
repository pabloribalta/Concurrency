/*
* Session 1, example 05:
*
* You  need  to  ensure  that  you’ve  called  either join() or detach() before a std::thread
* object is destroyed. If you’re detaching a thread, you can usually call detach() immediately
* after the thread has been started, so this isn’t a problem. 
*
* But if you’re intending to wait for the thread, you need to pick carefully the place in the 
* code where you call join(). This means that the call to join() is liable to be skipped if 
* an exception is thrown after the thread has been started but before the call to join().
*
* To avoid your application being terminated when an exception is thrown, you therefore need 
* to make a decision on what to do in this case. In general, if you were intending to call 
* join() in the non-exceptional case, you also need to call join() in the presence of an 
* exception to avoid accidental lifetime problems.
*
* The  following  program  shows  an  example  of  just such a scenario.
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

void dummyFunction()
{
  auto some_local_state = 0;

  BackgroundTask my_func(some_local_state);
  std::thread t(my_func);

  try {
    std::cout << "Doing something in current thread" << std::endl;
  } catch (...) {
    t.join(); /* (!) Exit by an exception */
    throw;
  }

  t.join(); /* (!) Exit normally */
}

int main()
{
  dummyFunction();

  return 0;
}

