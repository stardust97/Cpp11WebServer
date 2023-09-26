#ifndef __XTC__EPOLL__H
#define __XTC__EPOLL__H

#include <cstdint>
#include <memory>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

// #include "base/Channel.h" //BUG 头文件不应该互相包含！


namespace xtc{
class Channel; //利用前置声明代替循环包含的头文件

class Epoll {
public:
  Epoll();
  ~Epoll();

  void AddToEpoll(Channel* channel, uint32_t events);

  void RemoveFromEpoll(Channel* channel);

  void ModifyEpollEvent(Channel* channel, uint32_t eventes);

  void Wait( std::vector<xtc::Channel*>& active_events);

private:
  void fill_active_channels( std::vector<Channel*>& active_events, int32_t nums);
  
  static constexpr int32_t kInitEventListSize = 16;

  int32_t epoll_fd_;

  std::vector<epoll_event> events_; //发生事件的epoll_event

};



} // namespace xtc

#endif
