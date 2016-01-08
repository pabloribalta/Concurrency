/*
* Session 2, example 03:
*
* As  well  as checking that the member functions don’t pass out pointers or references to their callers,
* it’s  also  important  to  check  that  they  don’t  pass  such  pointers  or  references in to functions
* they call that aren’t under your control. This is just as dangerous: those functions might store the
* pointer or reference in a place where it can later be used without the protection  of  the  mutex.
*
* Particularly  dangerous  in  this  regard  are  functions  that  are  supplied at runtime via a function
* argument or other means,
*
*/
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

void doSomething()
{
  myWrapper.processData(maliciousFunction); /* (!) Pass in a malicious function */
  unprotected->doSomething(); /* (!) Unprotected access to protected data */
}

int main()
{
  doSomething();

  return 0;
}