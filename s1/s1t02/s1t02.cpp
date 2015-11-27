/*
* Session 1, example 02:
*
* In  this  case,  the  supplied  function  object  is copied into  the  storage  belonging  to  the
* newly created thread of execution and invoked from there. It’s therefore essential that
* the copy behave equivalently to the original, or the result may not be what’s expected.
* 
* One thing to consider when passing a function object to the thread constructor is
* to  avoid  what  is  dubbed  “C++’s  most  vexing  parse.”  If  you  pass  a  temporary  rather 
* than a named variable, then the syntax can be the same as that of a function declaration,
* in which case the compiler interprets it as such, rather than an object definition.
*
* For example:
*
*   std::thread my_thread(BackgroundTask());
*
* declares a function my_thread that takes a single parameter (of type pointer to a function
* taking  no  parameters  and  returning  a background_task object)  and  returns  a std::thread
* object, rather than launching a new thread. You can avoid this by naming your function object as
* shown previously, by using an extra set of parentheses, or by using the new uniform initialization
* syntax, for example:
*
*   std::thread my_thread((BackgroundTask()));
*   std::thread my_thread{ BackgroundTask() };
*/

#include <iostream>
#include <thread>

class BackgroundTask
{
public:
  void operator()() const
  {
    doSomething();
  }

private:
  static void doSomething()
  {
    std::cout << "Doing something" << std::endl;
  }
};

int main()
{
  BackgroundTask task;

  std::thread myThread(task);
  myThread.join();

  return 0;
}

