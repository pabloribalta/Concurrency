/*
* Session 1, example 12
*
* The move support in std::thread means  that  ownership  can  readily  be  transferred out of a function.
*
* Likewise,  if  ownership  should  be  transferred  into  a  function,  it  can  just  accept  an instance
* of std::thread by value as one of the parameters, as shown in the following example.
*
*/

#include <ostream>
#include <iostream>
#include <thread>

void doSomething()
{
  std::cout << "Doing something" << std::endl;
}

void doSomethingElse(int i)
{
  std::cout << "Do something else" << i << std::endl;
}

std::thread getThread1()
{
  return std::thread(doSomething);
}

std::thread getThread2()
{
  std::thread t(doSomethingElse, 42);

  return t;
}

void dummyFunction(std::thread t)
{
  t.join();
}

int main()
{
  dummyFunction(getThread1());
  dummyFunction(getThread2());

  dummyFunction(std::move(getThread1()));
  dummyFunction(std::move(getThread2()));

  return 0;
}

