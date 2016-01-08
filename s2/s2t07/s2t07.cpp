/*
* Session 2, example 07:
*
* In the following example, we show how through std::call_once() we can get rid of undefined
* behavior derived from teh antipattern Double-Checked Locking.
*
*/
#include <mutex>
#include <memory>
#include <iostream>

class Widget
{
public:
  void doSomething() const
  {
    std::cout << "Do something" << std::endl;
  }

};

std::shared_ptr<Widget> resource_ptr;
std::mutex resource_mutex;


void initializeResource()
{
  resource_ptr.reset(new Widget);
}

void threadSafeLazyInitializationWithMutex()
{
  std::unique_lock<std::mutex> lk(resource_mutex); /* (!) All threads are serialized here */
  if (!resource_ptr)
  {
    initializeResource(); /* (!) Only the initialization needs protection */
  }
  lk.unlock();

  resource_ptr->doSomething();
}

void undefinedBehavirourWithDoubleCheckedLocking()
{
  if (!resource_ptr) /* (!) Read outside the lock... bad idea */
  {
    std::lock_guard<std::mutex> lk(resource_mutex); /* (!) Lock is acquired */
    if (!resource_ptr)
    {
      initializeResource(); /* (!) The read outside of the lock is not synchronized with this write */
    }
  }
  resource_ptr->doSomething(); /* (!) This call may operate on incorrect values */
}

std::once_flag resource_flag;

void doSomething()
{
  std::call_once(resource_flag, initializeResource);
  resource_ptr->doSomething();
}

int main()
{
  threadSafeLazyInitializationWithMutex();

  undefinedBehavirourWithDoubleCheckedLocking(); /* (!) Oops */

  doSomething();

  return 0;
}