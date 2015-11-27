/*
* Session 1, example 09
*
* If you’re familiar with std::bind, the parameter-passing semantics will be unsurprising,
* because  both  the  operation  of  the std::thread constructor  and  the  operation of
* std::bind are defined in terms of the same mechanism. 
*
* This means that, for example, you can pass a member function pointer as the function, 
* provided you supply a suitable object pointer as the first argument.
*
*/
#include <thread>
#include <iostream>

class BackgroundTask
{
public:
  void doSomething()
  {
    std::cout << "Doing something in a separate thread" << std::endl;
  }

  void doSomethingElse(int i)
  {
    std::cout << "Doing something else  in a separate thread" << i << std::endl;
  }
};

int main()
{
  BackgroundTask myTask;

  std::thread t(&BackgroundTask::doSomething, &myTask); /* (!) This will invoke myTask.doSomething() in a new thread */
  t.join();

  std::thread t2(&BackgroundTask::doSomethingElse, &myTask, 42); /* (!) This will invoke myTask.doSomethingElse(42) in a new thread */
  t2.join();

  return 0;
}

