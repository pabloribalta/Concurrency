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

  void setName(std::string &name)
  {
    m_name = name;
  }

  std::string m_namegetName() const
  {
    return m_name;
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