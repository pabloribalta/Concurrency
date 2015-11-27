/*
* Session 1, example 03:
*
* One type of callable object that avoids the problem of the "C++'s most vexing parse" is
* a lambda expression.
*/


#include <iostream>
#include <thread>

void doSomething()
{
  std::cout << "Doing something" << std::endl;
}

void doSomethingElse()
{
  std::cout << "Doing something else" << std::endl;
}

int main()
{
  std::thread myThread([] {
    doSomething();
    doSomethingElse();
  });

  myThread.join();

  return 0;
}

