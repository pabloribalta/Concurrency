/*
 * Session 1, example 01:
 *
 * All this program does is write "Hello, concurrent world!" to the standard output stream. 
 */

#include <iostream>
#include <thread> /* (!) The functions and classes for managinig threads are declared in <thread> */

void hello() /* (!) This function will be spawned in a new thread */
{
  std::cout << "Hello concurrent world\n";
}

int main()
{
  std::thread t(hello); /* (!) The std::thread object t has hello() as its initial function */
  t.join(); /* (!) join() causes the thread running main() to wait for the std::thread associated with t */

  return 0;
}

