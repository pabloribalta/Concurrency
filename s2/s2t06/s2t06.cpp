/*
* Session 2, example 06:
*
* Because std::unique_lock instances don’t have to own their associated mutexes, the ownership of
* a  mutex  can  be  transferred  between  instances  by moving the  instances around. 
*
* In some cases such transfer is automatic, such as when returning an instance from a function, 
* and in other cases you have to do it explicitly by calling std::move().
*
* One possible use is to allow a function to lock a mutex and transfer ownership of that lock to
* the caller, so the caller can then perform additional actions under the pro tection  of  the  
* same  lock.
*
*/
#include <mutex>
#include <iostream>

std::mutex some_mutex;

void doSomething()
{
  std::cout << "Doing something" << std::endl;
}

void prepareData()
{
  std::cout << "Preparing data..." << std::endl;
}

std::unique_lock<std::mutex> getLock()
{
  std::unique_lock<std::mutex> lk(some_mutex);
  prepareData();
  return lk;
}
void processData()
{
  std::unique_lock<std::mutex> lk(getLock());
  doSomething();
}

int main()
{
  processData();

  return 0;
}