/**
* Session 1, example 07:
*
* Passing arguments to the callable object or function is funda mentally  as  simple  as  passing
* additional  arguments  to  the std::thread constructor.
*
* But it’s important to bear in mind that by default the arguments are copied into internal  storage,
* where  they  can  be  accessed  by  the  newly  created  thread  of  execution, even if the
* corresponding parameter in the function is expecting a reference.
*
* Here’s a simple example:
*
*   void f(int i,std::string const& s);
*   std::thread t(f, 3, "hello");
*
* This  creates  a  new  thread  of  execution  associated  with t ,  which  calls f(3,”hello”).
* Note that even though f takes a std::string as the second parameter, the string literal is passed
* as a char const* and converted to a std::string only in the context of the  new  thread.
*
* This  is  particularly  important  when  the  argument  supplied  is  a pointer to an automatic variable.
*
* In this case, it’s the pointer to the local variable buffer that’s passed through to the new thread ,
* and there’s a significant chance that the function oops will exit before the  buffer  has  been  converted
* to  a std::string on the new thread, thus leading to undefined  behavior.
*
* The  solution  is  to  cast  to std::string before passing  the  buffer to the std::thread constructor.
*/
#include <string>
#include <iostream>
#include <thread>

void doSomething(const std::string& s)
{
  std::cout << s << std::endl;
}

void oops(int some_param)
{
  char buffer[1024]; /* (!) Pointer to local variable buffer is passed through to the new thread */
  sprintf_s(buffer, 1024, "%i", some_param);

  std::thread t(doSomething, buffer); /** 
                                        * (!) Significant chance that the function oops 
                                        * will exit before the buffer has been converted to std::string
                                        */
  t.detach();
}

void notOops(int some_param)
{
  char buffer[1024];
  sprintf_s(buffer, 1024, "%i", some_param);

  std::thread t(doSomething, std::string(buffer)); /* (!) Using std::string avoids dangling pointer */
  t.detach();
}

int main()
{
  oops(42);

  notOops(42);

  return 0;
}

