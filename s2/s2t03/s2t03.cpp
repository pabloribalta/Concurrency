#include <string>
#include <mutex>

class Data
{
  Data(int a, const std::string& s)
    : a(a)
    , b(s)
  {
  }

  int a;
  std::string b;
public:
  void doSomething();
};
class WrapperData
{
  WrapperData(const Data& data, const std::mutex& m)
    : data(data)
    , m(m)
  {
  }

public:
  template<typename Function>
  void processData(Function func)
  {
    std::lock_guard<std::mutex> l(m);
    func(data); /* (!) Pass "protected" data to user-supplied function */
  }

private:
  Data data;
  std::mutex m;
};

Data* unprotected;
WrapperData myWrapper;

void maliciousFunction(Data& protected_data)
{
  unprotected = &protected_data;
}

void lazyInitializationWithCallOnce()
{
  myWrapper.processData(maliciousFunction); /* (!) Pass in a malicious function */
  unprotected->doSomething(); /* (!) Unprotected access to protected data */
}

int main()
{
  lazyInitializationWithCallOnce();

  return 0;
}