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