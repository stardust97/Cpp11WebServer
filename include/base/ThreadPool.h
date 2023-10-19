#ifndef __XTC__THREAD_POOL__H__
#define __XTC__THREAD_POOL__H__

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <type_traits>
#include <memory>


namespace xtc{
  using Task = std::function<void(void)>;
class ThreadPool {

public:
  ThreadPool(int32_t thread_num = std::thread::hardware_concurrency());
  ~ThreadPool();

  template<typename F, typename ... Args>
  auto AddTask(F&& f, Args&&... args) -> std::future<typename std::result_of_t<F(Args...)>>;

private:
  void work();
  int32_t thread_nums_;
  std::vector<std::thread> threads_;
  std::queue<Task> tasks_;
  std::mutex mutex_;
  std::condition_variable cv_;
  bool stopped_;
};

  template<typename F, typename ... Args>
  auto ThreadPool::AddTask(F&& f, Args&&... args) -> std::future<typename std::result_of_t<F(Args...)>> {
    using return_type = typename std::result_of_t<F(Args...)>;  //返回值类型
    auto task = std::make_shared<std::packaged_task<return_type()> > ( 
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)    // 完美转发参数
    );
    std::future<return_type> res = task->get_future();  // 使用期约
    {
      std::unique_lock<std::mutex> lock(mutex_);
      tasks_.emplace([task]() { (*task)(); });
    }
    cv_.notify_one();
    return res;
  }



} // namespace xtc



#endif // __XTC__THREAD_POOL__H__
