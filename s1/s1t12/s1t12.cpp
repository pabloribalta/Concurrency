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

