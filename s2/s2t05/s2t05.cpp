/*
* Session 2, example 05:
*
* std::unique_lock provides  a  bit  more  flexibility  than std::lock_guard by  relaxing the  invariants;
* a std::unique_lock instance  doesn’t  always  own  the  mutex  that  it’s associated with.
*
* First off, just as you can pass std::adopt_lock as a second argument to the constructor to have the lock
* object manage the lock on a mutex, you can also pass std::defer_lock as  the  second  argument  to  indicate
* that  the  mutex  should remain  unlocked  on  construction.
*
* The  lock  can  then  be  acquired  later  by  calling std::lock() on  the std::unique_lock object (not the  mutex)
* or  by  passing  the std::unique_lock object  itself  to std::lock().
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
    std::unique_lock<std::mutex> lock_a(lhs.m_mutex, std::defer_lock);
    std::unique_lock<std::mutex> lock_b(rhs.m_mutex, std::defer_lock);

    std::lock(lock_a, lock_b);

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