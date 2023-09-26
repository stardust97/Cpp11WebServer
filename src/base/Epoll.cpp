#include "base/Epoll.h"

#include <cstring>

#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "util/error_process.h"

namespace xtc{


Epoll::Epoll(): epoll_fd_(epoll_create1(0)), events_(kInitEventListSize) {
  errif(epoll_fd_ == -1, "epoll_create failed");
}

Epoll::~Epoll() {
  if (epoll_fd_!= -1) {
    ::close(epoll_fd_);
  }
}

void Epoll::AddToEpoll(Channel* channel, uint32_t events){
  struct epoll_event ev;
  bzero(&ev, sizeof(struct epoll_event));
  ev.events = events;
  // NOTE 这里使用ptr来记录发生事件的Channel，而不是fd
  ev.data.ptr = static_cast<void*> (channel);
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel->GetFd(), &ev);   
}

void Epoll::RemoveFromEpoll(Channel* channel){
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel->GetFd(), NULL);   
}

void Epoll::ModifyEpollEvent(Channel* channel, uint32_t events){
  struct epoll_event ev;
  bzero(&ev, sizeof(struct epoll_event));
  ev.events = events;
  ev.data.ptr =static_cast<void*> (channel) ;  
  epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, channel->GetFd(), NULL);
}

void Epoll::Wait(std::vector<Channel*>& active_events) {
  // std::vector<int> epoll
  int32_t nfds = epoll_wait(epoll_fd_, events_.data(), events_.size(), -1);
  fill_active_channels(active_events, nfds);
}

void Epoll::fill_active_channels( std::vector<Channel*>& active_events, int32_t nums) {
  for (int32_t i = 0; i < nums; i++) {
    Channel* ch = static_cast<Channel*>(events_[i].data.ptr);
    ch ->SetRevents(events_[i].events);
    active_events.push_back(ch);
  }
}



} // namespace xtc
