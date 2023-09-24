#ifndef __XTC__EPOLL__H
#define __XTC__EPOLL__H

#include <cstdint>
#include <memory>
#include <vector>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "base/Channel.h"

namespace xtc{

class Epoll {
public:
  Epoll();
  ~Epoll();

  void AddToEpoll(int32_t fd, uint32_t events);

  void RemoveFromEpoll(int32_t fd);

  void ModifyEpollEvent(int32_t fd, uint32_t eventes);

  void Wait(std::vector<epoll_event>& active_events); //MARK 弃用
  void Wait( std::vector<xtc::Channel*>& active_events);

private:
  void fillActiveEvents( std::vector<Channel*>& active_events, int32_t nums);
  static constexpr int32_t kInitEventListSize = 16;

  int32_t epoll_fd_;

  std::vector<epoll_event> events_; //发生事件的

};



} // namespace xtc

#endif
