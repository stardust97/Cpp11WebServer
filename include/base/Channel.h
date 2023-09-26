#ifndef __XTC__CHANNEL__H__
#define __XTC__CHANNEL__H__



#include <memory>
#include <functional>
#include "base/Epoll.h"

namespace xtc{

class Channel {
  constexpr static int32_t kNoneEvent = 0;
  constexpr static int32_t kReadEvent = EPOLLIN | EPOLLPRI;
  constexpr static int32_t KETReadEvent = EPOLLIN | EPOLLPRI | EPOLLET;
  constexpr static int32_t kWriteEvent = EPOLLOUT;

public:
  using ReadEventCallback = std::function<void ()>;
  using EventCallback = std::function<void ()>;

  Channel() = delete;
  Channel(Epoll* ep, int32_t fd);
  ~Channel() = default;

  const int32_t& GetFd() {return fd_;};
  void SetRevents(uint32_t events);
  uint32_t GetActiveEvents();

  void EnableReading();
  void EnableETReading();
  void DisableReading();
  void EnableWriting();
  void DisableWriting();
  void DisableAll();

  void SetReadCallback(ReadEventCallback const& cb);
  void SetWriteCallback(EventCallback const& cb);
  void SetCloseCallback(EventCallback const& cb);
  void SetErrorCallback(EventCallback const& cb);
  
private:
  void set_fd_noblocked();

  int32_t fd_; //Channel所管理的fd
  std::shared_ptr<Epoll> epoll_; // Channel被哪个epoll所监听
  bool is_polled_; // 指示是否已经被加入到epoll中监听
  uint32_t events_; // 本Channel所关心的事件
  uint32_t revents_; // epoll返回的实际发生的事件

  ReadEventCallback read_callback_;
  EventCallback write_callback_;
  EventCallback close_callback_;
  EventCallback error_callback_;

};



} // namespace xtc
#endif //__XTC__CHANNEL__H__
