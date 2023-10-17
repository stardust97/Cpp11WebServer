#ifndef __XTC__THREAD_POOL__H__
#define __XTC__THREAD_POOL__H__

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>


namespace xtc{

class ThreadPool {
  using Task = std::function<void(void)>;
public:
  ThreadPool();
  ~ThreadPool();
  void AddTask(Task const& task);

private:
  void work();
  int32_t thread_nums_;
  std::vector<std::thread> threads_;
  std::queue<Task> tasks_;
  std::mutex mutex_;
  std::condition_variable cv_;
  bool stopped_;
};


  
} // namespace xtc



#endif // __XTC__THREAD_POOL__H__
