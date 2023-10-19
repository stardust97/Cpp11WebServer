#include "base/ThreadPool.h"
#include "util/Logger.h"


namespace xtc{
ThreadPool::ThreadPool(int32_t thread_num) : 
    thread_nums_(thread_num) {
  threads_ = std::vector<std::thread>(thread_nums_); //TODO move
  LOG4CXX_INFO(Logger::GetLogger(), "create threads");
  for (int i = 0; i< thread_nums_; ++i) {
    threads_[i] = std::thread(std::bind(&ThreadPool::work, this));
  }
  stopped_ = false;
}

ThreadPool::~ThreadPool() {
  stopped_ = true;
  for(auto& thread: threads_) {
    thread.join();
    LOG4CXX_DEBUG(Logger::GetLogger(), "thread joined");
  }

}


void ThreadPool::work() {
  LOG4CXX_INFO(Logger::GetLogger(), "work start");
  while(!stopped_) { // TODO loop为死循环，无法退出
    Task task;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      while(tasks_.empty()) { // 避免虚假唤醒
        cv_.wait(lock);
      }
      task = tasks_.front();
      tasks_.pop();
    }
    LOG4CXX_DEBUG(Logger::GetLogger(), "task start");
    task();
  }

}






} // namespace xtc
