/**
* Session 1, example 08:
*
* It’s also possible to get the reverse scenario: the object is copied, and what you
* wanted was a reference. This might happen if the thread is updating a data structure
* that’s passed in by reference.
*
* Although updateWidget expects the parameter to be passed by reference,  the std::thread
* constructor doesn’t  know  that;  it’s  oblivious  to  the types of the arguments expected
* by the function and blindly copies the supplied values. 
*
* When  it  calls updateWidget,  it  will  end  up  passing  a  reference  to the internal
* copy of data and not a reference to data itself. 
*
* Consequently, when the thread finishes, these updates will be discarded as the internal
* copies of the supplied arguments  are  destroyed,  and will  be  passed  an  unchanged
* data rather  than  a  correctly  updated  version.  
*
* For  those  familiar  with std::bind , the solution will be readily apparent: you need
* to wrap the arguments that really need to be references in std::ref.
*/

#include <string>
#include <iostream>
#include <thread>

struct Widget
{
  Widget() : m_data(0)
  {
  }

  void setData(int i)
  {
    m_data = i;
  }

  int getData() const
  {
    return m_data;
  }

private:
  int m_data;
};

void updateWidget(Widget& w) /* (!) Expects the second parameter to be passed by reference */
{
  w.setData(42);
}

void oops(Widget& w)
{
  std::thread t(updateWidget, w); /* (!) Oblivious pass by copy */
  t.join();

  std::cout << w.getData() << std::endl; /* (!) Internal thread storage is destroyed, changes are lost */
}

void notOops(Widget& w)
{
  std::thread t(updateWidget, std::ref(w)); /* (!) Explicit pass by reference */
  t.join();

  std::cout << w.getData() << std::endl;
}

int main()
{
  Widget w;

  oops(w);
  notOops(w);

  return 0;
}

