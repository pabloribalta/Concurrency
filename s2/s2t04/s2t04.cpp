#include <mutex>
#include <iostream>
#include <string>

class Widget
{
public:
  explicit Widget(std::string &name)
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