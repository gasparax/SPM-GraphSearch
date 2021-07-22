#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <vector>
#include <chrono>
#include <cstddef>
#include <math.h>
#include <string>

//
// needed a blocking queue
// here is a sample queue
//

template <typename T>
class myqueue
{
private:

  std::mutex              d_mutex;
  std::condition_variable d_condition;
  
public:

  myqueue(std::string s) { std::cout << "Created " << s << " queue " << std::endl;  }
  myqueue() {}
  std::deque<T> d_queue;

  void push(T const& value) {
    {
      std::unique_lock<std::mutex> lock(this->d_mutex);
      d_queue.push_front(value);
    }
    this->d_condition.notify_one();
  }
  
  T pop() {
    std::unique_lock<std::mutex> lock(this->d_mutex);
    this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
    T rc(std::move(this->d_queue.back()));
    this->d_queue.pop_back();
    return rc;
  }

  bool empty(){
    std::unique_lock<std::mutex> lock(this->d_mutex);
    return this->d_queue.empty();
  }

};



//
// needed something to represent the EOS
// here we use null
//
