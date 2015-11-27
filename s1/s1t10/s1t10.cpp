/*
* Session 1, example 10
*
* Another interesting scenario for supplying arguments is where the arguments can’t be copied but
* can only be moved: the data held within one object is transferred over  to  another,  leaving
* the  original  object  “empty.”  An  example  of  such  a  type  is std::unique_ptr ,  which
* provides  automatic  memory  management  for  dynamically allocated objects. 
*
* Only one std::unique_ptr instance can point to a given object at a time, and when that instance
* is destroyed, the pointed-to object is deleted. The move constructor and move  assignment  operator
* allow  the  ownership  of  an  object  to  be  transferred around between std::unique_ptr instances.
*
* Such  a  transfer  leaves  the  source  object  with  a NULL pointer.  This  moving  of  values  
* allows  objects  of  this  type  to  be  accepted  as  function parameters or returned from functions.
* Where the source object is a temporary, the move  is  automatic,  but  where  the  source  is  a 
* named  value,  the  transfer  must  be requested directly by invoking std::move().
*
* Note: Because of a bug in MSVC2012, MSVC2013 (and possibly MSVC2015) std::thread does not use perfect 
* forwarding directly, as storing a reference to data (temporary or not) in the originating thread and
* using it in the spawned thread would be extremely error prone and dangerous.
*
* Instead, it copies each argument into decay_t<?>'s internal data.
*
* The bug is that when it calls the worker function, it simply passes that internal copy to the procedure.
* Instead, it should move that internal data into the call.
*
* For more information: 
*   - http://stackoverflow.com/questions/28330471/visual-studio-implementation-of-move-semantics-and-rvalue-reference
*/

#include <thread>
#include <memory>
#include <iostream>

class Widget
{
public:
  Widget() : m_data(0)
  {
  }

  void prepareData(int i)
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

void processWidget(std::unique_ptr<Widget>/*&*/ widget) /* (!) Bug of Visual Studio compiler. Uncomment & to make it compiling. */
{
  std::cout << widget->getData() << std::endl;
}

int main()
{
  std::unique_ptr<Widget> widget(new Widget());
  widget->prepareData(42);

  std::thread t(processWidget, std::move(widget));
  t.join();

  return 0;
}

