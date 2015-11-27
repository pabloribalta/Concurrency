/*
* Session 1, task 11
*
* As described in the previous  section,  many  resource-owning  types  in  the  C++  Standard  Library  such
* as std::ifstream and std::unique_ptr are movable but not copyable, and std::thread is one of them.
*
* This means that the ownership of a particular thread of execution can be moved between std::thread instances,
* as in the following example.
*
* The example shows the creation of two threads of execution and the transfer of ownership of those threads among
* three  std::thread  instances, t1, t2, and t3.
*
*/

#include <thread>
#include <iostream>

void doSomething()
{
  std::cout << "Doing something" << std::endl;
}

void doSomethingElse()
{
  std::cout << "Do something else" << std::endl;
}


int main()
{
  std::thread t1(doSomething); /* (!) A new std::thread is started and associated with t1 */

  std::thread t2 = std::move(t1); /* (!) Transfer ownership of t1 upon construction of t2 */

  t1 = std::thread(doSomethingElse); /* (!) Transfer of ownership doesn't require a call to std::move() because the owner is a temporary */

  std::thread t3; /* (!) Default constructed without any thread association */

  t3 = std::move(t2); /* (!) Ownership of thread currently in t2 is transferred to t3 */

  /* t1 = std::move(t3);*/ /**
                            * (!) t1 already holds a thread so this assignment will terminate the program, as the destruction of the currently 
                            * owned object, will develop into a call to std::terminate() 
                            */

  t1.join();
  t3.join();

  return 0;
}

