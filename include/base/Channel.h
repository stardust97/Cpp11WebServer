#ifndef __XTC__CHANNEL__H__
#define __XTC__CHANNEL__H__



#include <memory>
#include <functional>
#include "base/Epoll.h"

namespace xtc{

class Channel {
public:
  using ReadEventCallback = std::function<void ()>;
  using EventCallback = std::function<void ()>;

  Channel() = delete;
  Channel(Epoll* ep, int32_t fd);
  ~Channel() = default;

  void SetRevents(uint32_t events);
  
  void EnableReading();
  void DisableReading();
  void EnableWriting();
  void DisableWriting();
  void DisableAll();

  void SetReadCallback(ReadEventCallback const& cb);
  void SetWriteCallback(EventCallback const& cb);
  void SetCloseCallback(EventCallback const& cb);
  void SetErrorCallback(EventCallback const& cb);
  
private:
  int32_t fd_;
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
