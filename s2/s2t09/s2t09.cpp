/*
* Session 2, example 09:
*
* Scraping an interface for a thread-safe stack.
*/

#include <exception>
#include <memory> 
#include <stack>
#include <mutex>

struct empty_stack : std::exception
{
  const char* what() const throw() override;
};

template<typename T>
class ThreadSafeStack
{
public:
  ThreadSafeStack()
  {
  }

  ThreadSafeStack(const ThreadSafeStack& other)
  {
    std::lock_guard<std::mutex> lock(other.m);
    data = other.data;
  }

  ThreadSafeStack& operator=(const ThreadSafeStack&) = delete; /* (!) Assignment operator is deleted */

  void push(T new_value)
  {
    std::lock_guard<std::mutex> lock(m);
    data.emplace(new_value);
  }

  std::shared_ptr<T> pop()
  {
    std::lock_guard<std::mutex> lock(m);

    if (data.empty()) { /* (!) Check for empty before trying to pop a value */
      throw empty_stack();
    }

    const std::shared_ptr<T> ret(std::make_shared<T>(data.top())); /* (!) Allocate return value before modifying the stack */
    data.pop();

    return ret;
  }

  void pop(T& value)
  {
    std::lock_guard<std::mutex> lock(m);

    if (data.empty()) { /* (!) Check for empty before trying to pop a value */
      throw empty_stack();
    }

    value = data.top();
    data.pop();
  }

  bool empty() const
  {
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
  }

private:
  std::stack<T> data;
  mutable std::mutex m;
};

int main()
{
  return 0;
}

