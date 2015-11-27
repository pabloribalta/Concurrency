/*
* Session 1, example 06:
*
* The use of try/catch blocks is verbose, and it’s easy to get the scope  slightly  wrong,
* so  this  isn’t  an  ideal  scenario.  If  it’s  important  to  ensure  that the thread 
* must complete before the function exits—whether because it has a reference  to  other  
* local  variables  or  for  any  other  reason—then  it’s  important  to  ensure this  is
* the  case  for  all  possible  exit  paths,  whether  normal  or  exceptional,  and  it’s
* desirable to provide a simple, concise mechanism for doing so.
*
* One way of doing this is to use the standard Resource Acquisition Is Initialization (RAII)
* idiom and provide a class that does the join() in its destructor, greatly simplifying the code.
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

class ThreadGuard
{
  std::thread& t;
public:
  explicit ThreadGuard(std::thread& t_) :
    t(t_)
  {
  }

  ~ThreadGuard()
  {
    if (t.joinable()) /* (!) In this case, it is important to verify that the thread is joinable before calling join() */ {
      t.join(); /* (!) Join can be only called once */
    }
  }

  ThreadGuard(ThreadGuard const&) = delete; /* (!) Prevent copying a ThreadGuard deleting copy constructor and assignment operators */
  ThreadGuard& operator=(ThreadGuard const&) = delete;
};

void dummyFunction()
{
  auto some_local_state = 0;

  BackgroundTask myFunc(some_local_state);

  std::thread t(myFunc);
  ThreadGuard g(t);

  std::cout << "Doing something in current thread" << std::endl;
} /* (!) ThreadGuard destructor cleans up after the std::thread object is destroyed */

int main()
{
  dummyFunction();

  return 0;
}

