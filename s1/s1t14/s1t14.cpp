#include <iostream>
#include <thread>

std::thread::id masterThreadID;

void masterThreadWork()
{
  std::cout << "Doing master thread work" << std::endl;
}

void commonThreadWork()
{
  std::cout << "Doing common thread work" << std::endl;
}

void randomAlgorithm()
{
  if (std::this_thread::get_id() == masterThreadID) {
    masterThreadWork();
  } else {
    commonThreadWork();
  }
}

int main()
{
  masterThreadID = std::this_thread::get_id();

  std::thread t(randomAlgorithm);
  t.join();

  std::thread t2(randomAlgorithm);
  t2.join();

  randomAlgorithm();

  return 0;
}

