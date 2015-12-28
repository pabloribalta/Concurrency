/*
* Session 2, example 02:
*
* In the following example there is a regular std::list which is intended to be made thread safe, and it’s protected with a corresponding
* instance  of std::mutex.  The  use  of  std::lock_guard<std::mutex>  in  add() and again in contains() means that the accesses in these
* functions  are  mutually  exclusive: contains() will  never  see  the  list  partway through a modification by  add().
*
* If one of the member  functions  returns  a  pointer  or  reference  to  the  protected  data,  then  it doesn’t matter that the member
* functions all lock the mutex in a nice orderly fashion, because you’ve just blown a big hole in the protection. 
*
* Any code that has access to that pointer or reference can now access (and potentially modify) the protected data without locking the mutex.
* Protecting  data  with  a  mutex  therefore  requires  careful  interface  design,  to ensure  that  the  mutex  is  locked  before  there’s
* any  access  to  the  protected  data  and that there are no backdoors.
*
*/
#include <list>
#include <mutex>
#include <thread>
#include <iostream>

/* Scoped thread class from Session 1*/
class ThreadGuard
{
  std::thread t;

public:
  explicit ThreadGuard(std::thread& t_)
    : t(std::move(t_))
  {
    if (!t.joinable()) {
      throw std::logic_error("No thread");
    }
  }

  ~ThreadGuard()
  {
    t.join();
  }

  ThreadGuard(ThreadGuard const&) = delete;
  ThreadGuard& operator=(ThreadGuard const&) = delete;
};

struct ThreadSafeLinkedList
{
  explicit ThreadSafeLinkedList()
  {
  }

  void add(int value)
  {
    std::lock_guard<std::mutex> guard(myMutex); /* (!) Lock the instance until adding is completed */
    myList.emplace_back(value);
  }

  bool contains(int value)
  {
    std::lock_guard<std::mutex> guard(myMutex); /* (!) Lock the instance until finding is completed */
    return std::find(myList.begin(), myList.end(), value) != myList.end();
  }

  std::list<int>& getList() /* (!) This function will expose the member we wanted to protect, breaking the interface */
  {
    return myList;
  }

private:
  std::list<int> myList; /* (!) Data structure which is not thread safe */
  std::mutex myMutex; /* (!) Protects myList instance */
};

int main()
{
  ThreadSafeLinkedList mySafeLinkedList;

  ThreadGuard addElementsTask(std::thread([&mySafeLinkedList] {
    for (auto i = 0; i < 5; ++i) {
      mySafeLinkedList.add(i);

    }
  }));

  ThreadGuard findElementsTask(std::thread([&mySafeLinkedList] {
    for (auto i = 0; i < 5; ++i) {
      if (mySafeLinkedList.contains(i)) {
        std::cout << i << " is in the list" << std::endl;
      } else {
        std::cout << i << " is NOT in the list" << std::endl;
      }
    }
  }));

  auto& notSafeLinkedList = mySafeLinkedList.getList(); /* (!) Defeats the whole purpose of the locking */

  return 0;
}

