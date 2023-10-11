#include "base/Channel.h"

#include <memory>
#include <fcntl.h>

#include "base/Channel.h"
#include "base/Epoll.h"
#include "base/Channel.h"


namespace xtc{  

Channel::Channel(EventLoop* ep, int32_t fd) :
    loop_(std::shared_ptr<EventLoop>(ep)), fd_(fd), is_polled_(false) {

}

void Channel::SetRevents(uint32_t events) {
  revents_ = events;
}

void Channel::HandleEvents() {
  if (revents_ & KReadEvent) {
    read_callback_(this);
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
  events_ |= KReadEvent;
  update_channel_events();
}

void Channel::EnableETReading() {
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
}

} // namespace xtc
