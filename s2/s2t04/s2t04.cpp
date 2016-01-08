/*
* Session 2, example 04:
*
* The common advice for avoiding deadlock is to always lock the two mutexes in the same order: if you always lock mutex
* A before mutex B, then you’ll never deadlock.
*
* Sometimes  this  is  straightforward,  because  the  mutexes  are  serving  different  purposes, but other times it’s
* not so simple, such as when the mutexes are each protecting  a  separate  instance  of  the  same  class.  
*
* Consider,  for  example,  an  operation  that exchanges  data  between  two  instances  of  the  same class; in order
* to ensure that the data  is  exchanged  correctly,  without  being  affected  by  concurrent  modifications, the mutexes
* on  both  instances  must  be  locked.  However,  if  a  fixed  order  is  chosen (for  example,  the  mutex  for  the
* instance  supplied  as  the  first  parameter,  then  the mutex  for  the  instance  supplied  as  the  second  parameter),
* this  can  backfire:  all  it takes is for two threads to try to exchange data between the same two instances with
* the parameters swapped, and you have deadlock!
*
* Thankfully, the C++ Standard Library has a cure for this —in the form of std::lock()— a  function  that  can  lock  two  
* or  more  mutexes  at  once  without  risk  of  deadlock.
*
*/
#include <mutex>
#include <iostream>
#include <string>

class Widget
{
public:
  explicit Widget(std::string &&name)
    : m_name(name)
  {    
  }

  std::mutex m_mutex;
  std::string m_name;
};

void swap(Widget &lhs, Widget &rhs)
{
  if (&lhs != &rhs) {
    std::lock(lhs.m_mutex, rhs.m_mutex); /* (!) Acquire both mutexes at the same time */

    std::lock_guard<std::mutex> lock_a(lhs.m_mutex, std::adopt_lock); /* (!) Construct std::lock_guard indicating that mutex is already locked */
    std::lock_guard<std::mutex> lock_b(rhs.m_mutex, std::adopt_lock); /* (!) Construct std::lock_guard indicating that mutex is already locked */

    std::swap(lhs.m_name, rhs.m_name);
  }
}

int main()
{
  Widget w1(std::string("Widget 1"));
  Widget w2(std::string("Widget 2"));

  std::cout << w1.m_name << std::endl;
  std::cout << w2.m_name << std::endl;

  swap(w1, w2);

  std::cout << w1.m_name << std::endl;
  std::cout << w2.m_name << std::endl;

  return 0;
}