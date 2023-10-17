#include "base/ThreadPool.h"
#include "util/Logger.h"


namespace xtc{
ThreadPool::ThreadPool() :  thread_nums_(std::thread::hardware_concurrency()) {
  threads_ = std::vector<std::thread>(thread_nums_); //TODO move
  for (int i = 0; i< thread_nums_; ++i) {
    threads_[i] = std::thread(std::bind(&ThreadPool::work, this));
  }
  stopped_ = false;
}

ThreadPool::~ThreadPool() {
  stopped_ = true;
  for(auto& thread: threads_) {
    thread.join();
  }

}

void ThreadPool::AddTask(Task const& task) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push(task);
  }
  cv_.notify_one();
}

void ThreadPool::work() {
  while(!stopped_) {
    Task task;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      while(tasks_.empty()) { // 避免虚假唤醒
        cv_.wait(lock);
      }
      task = tasks_.front();
      tasks_.pop();
    }
    task();
  }

}






} // namespace xtc
