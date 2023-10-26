#include "base/Epoll.h"

#include <cstring>

#include <unistd.h> 
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "util/error_process.h"
#include "base/Channel.h"
#include "util/Logger.h"

namespace xtc{

Epoll::Epoll(): epoll_fd_(epoll_create1(0)), events_(kInitEventListSize) {
  if(epoll_fd_ == -1 ) {
    LOG4CXX_ERROR(Logger::GetLogger(), "epoll create failed");
  }
  polled_nums_ = 0;
}

Epoll::~Epoll() {
  if (epoll_fd_!= -1) {
    ::close(epoll_fd_);
  }
}

void Epoll::Wait(std::vector<Channel*>& active_events) {
  int32_t nfds = epoll_wait(epoll_fd_, events_.data(), events_.size(), -1);
  fill_active_channels(active_events, nfds);
}

void Epoll::AddToEpoll(Channel* channel, uint32_t events){
  struct epoll_event ev;
  bzero(&ev, sizeof(struct epoll_event));
  ev.events = events;
  // NOTE 这里使用ptr来记录发生事件的Channel，而不是fd
  ev.data.ptr = static_cast<void*> (channel);
  LOG4CXX_DEBUG(Logger::GetLogger(), " client fd: " << channel->GetFd() << " add to epoll, events: " \
      << channel->GetEvents());
  epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, channel->GetFd(), &ev);   
  polled_nums_++;
}

void Epoll::RemoveFromEpoll(Channel* channel){
  LOG4CXX_INFO(Logger::GetLogger(), " client fd: " << channel->GetFd() << " remove from epoll ");
  epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, channel->GetFd(), NULL);
}

void Epoll::ModifyEpollEvent(Channel* channel, uint32_t events){
  LOG4CXX_INFO(Logger::GetLogger(), " client fd: " << channel->GetFd() << " modify to epoll, events: " \
      << channel->GetEvents());
  struct epoll_event ev;
  bzero(&ev, sizeof(struct epoll_event));
  ev.events = events;
  ev.data.ptr =static_cast<void*> (channel) ;  
  epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, channel->GetFd(), NULL);
}

void Epoll::UpdateChannel(Channel* channel) {
  LOG4CXX_INFO(Logger::GetLogger(), " client fd: " << channel->GetFd() << " uptate to epoll, events: " \
      << channel->GetEvents());
  uint32_t events = channel -> GetEvents();
  if (channel -> GetIsPolled()) {
    ModifyEpollEvent(channel, events);
  } else {
    AddToEpoll(channel, events);
  }
}

void Epoll::fill_active_channels(std::vector<Channel*>& active_events, int32_t nums) {
  for (int32_t i = 0; i < nums; i++) {
    Channel* ch = static_cast<Channel*>(events_[i].data.ptr);
    ch ->SetRevents(events_[i].events);
    active_events.push_back(ch);
  }
}



} // namespace xtc
