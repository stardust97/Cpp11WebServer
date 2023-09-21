#include "base/Epoll.h"

#include <cstring>

#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "util/error_process.h"

namespace xtc{


Epoll::Epoll(): epoll_fd_(epoll_create1(0)),events_(kInitEventListSize) {
  errif(epoll_fd_ == -1, "epoll_create failed");
}

Epoll::~Epoll() {
  if (epoll_fd_!= -1) {
    ::close(epoll_fd_);
  }
}

// TODO ET模式下的非阻塞文件描述符
void Epoll::AddToEpoll(int32_t fd, uint32_t events){
  struct epoll_event ev;
  bzero(&ev, sizeof(struct epoll_event));
  ev.events = events;
  ev.data.fd = fd;
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);   
}

void Epoll::RemoveFromEpoll(int32_t fd){
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, NULL);   
}

// TODO
void Epoll::ModifyEpollEvent(int32_t fd, uint32_t eventes){

}

void Epoll::Wait(std::vector<epoll_event>& active_events) {
  int32_t nfds = epoll_wait(epoll_fd_, events_.data(), events_.size(),-1);
  errif(nfds != 0, "epoll_wait failed");
  fillActiveEvents(nfds, active_events);
}

void Epoll::fillActiveEvents(int32_t nums, std::vector<epoll_event>& active_events) {
  for (int32_t i = 0; i < nums; i++) {
    active_events.push_back(events_[i]);
  }
}


} // namespace xtc
