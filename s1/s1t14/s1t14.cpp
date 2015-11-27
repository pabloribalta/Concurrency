/*
* Session 1, example 14
*
* Thread  identifiers  are  of  type std::thread::id and  can  be  retrieved  in  two  ways.
*
* First,  the  identifier  for  a  thread  can  be  obtained  from  its  associated std::thread
* object  by  calling  the get_id()  member  function.  If  the  std::thread   object  doesn’t
* have  an  associated  thread  of  execution,  the  call  to  get_id()  returns  a  default-constructed 
* std::thread::id  object, which indicates “not any thread”. 
*
* Alternatively, the identifier for the current thread can be obtained by calling std::this_thread::get_id(),
* which is also defined in the <thread> header.
*
* Objects of type std::thread::id  can  be  freely  copied  and  compared;  they wouldn’t be of much use
* as identifiers otherwise. If two objects of type  std::thread::id are equal, they represent the same
* thread, or both are holding the “not any thread” value. 
*
* If two objects aren’t equal, they represent different threads, or one represents a thread and the other
* is holding the “not any thread” value.
*
*/
#include <iostream>
#include <thread>

std::thread::id masterThreadID;

void masterThreadWork()
{
  std::cout << "Doing master thread work" << std::endl;
}

void commonThreadWork()
{
  std::cout << "Doing common thread work" << std::endl;
}

void randomAlgorithm()
{
  if (std::this_thread::get_id() == masterThreadID) {
    masterThreadWork();
  } else {
    commonThreadWork();
  }
}

int main()
{
  masterThreadID = std::this_thread::get_id();

  std::thread t(randomAlgorithm);
  t.join();

  std::thread t2(randomAlgorithm);
  t2.join();

  randomAlgorithm();

  return 0;
}

