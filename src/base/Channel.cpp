#include "base/Channel.h"

#include <memory>
#include <fcntl.h>

#include "base/Channel.h"
#include "base/Epoll.h"
#include "base/Channel.h"
#include "util/Logger.h"

namespace xtc{  

Channel::Channel(EventLoop* loop, int32_t fd) :fd_(fd), is_blocked_(true), 
    loop_(loop), is_polled_(false),events_(0), revents_(0) {
}

Channel::~Channel() {
  if(is_polled_) {
    loop_ -> DisableChannel(this);
  }
  close(fd_);
  LOG4CXX_INFO(Logger::GetLogger(),"channel closed, fd: " << fd_);
}

void Channel::SetRevents(uint32_t events) {
  revents_ = events;
  LOG4CXX_TRACE(Logger::GetLogger(), "set Revents,channel fd is " << fd_ << " concerned event is " << events_);
}

void Channel::HandleEvents() {
  if (revents_ & KReadEvent) {
    read_callback_();
  } 
  if (revents_ & KWriteEvent) {
    write_callback_();
  } 
  if (revents_ & KCloseEvent) {
    close_callback_();
  } 
  if (revents_ & KErrorEvent) {
    error_callback_();
  }
}

void Channel::EnableReading() {
  LOG4CXX_INFO(Logger::GetLogger(), "Channel fd "<< fd_ << " enable reading");
  events_ |= KReadEvent;
  update_channel_events();
}

void Channel::EnableETReading() {
  LOG4CXX_INFO(Logger::GetLogger(), "Channel enable ET reading");
  events_ |= KETReadEvent;
  set_fd_noblocked();
  update_channel_events();
}

void Channel::DisableReading() {
  
}

void Channel::EnableWriting() {

}

void Channel::DisableWriting() {

}

void Channel::DisableAll() {

}

void Channel::SetReadCallback(ReadEventCallback const& cb) {
  LOG4CXX_DEBUG(Logger::GetLogger(), "channel fd "<< fd_ << " register read callback");
  read_callback_ = cb;
}

void Channel::SetWriteCallback(EventCallback const& cb) {
  write_callback_ = cb;
}

void Channel::SetCloseCallback(EventCallback const& cb) {
  close_callback_ = cb;
}

void Channel::SetErrorCallback(EventCallback const& cb) {
  error_callback_ = cb;
}


void Channel::update_channel_events() {
  loop_ -> UpdateChannel(this);
  is_polled_ = true;
}


void Channel::set_fd_noblocked() {
  int flags = fcntl(fd_, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(fd_, F_SETFL, flags);
  is_blocked_ = false;
}

} // namespace xtc
