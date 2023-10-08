#include "base/Channel.h"

#include <memory>
#include <fcntl.h>

#include "base/Channel.h"
#include "base/Epoll.h"


namespace xtc{  

Channel::Channel(Epoll* ep, int32_t fd) :
    epoll_(std::shared_ptr<Epoll>(ep)), fd_(fd) {

}

void Channel::SetRevents(uint32_t events){
  revents_ = events;
}
uint32_t Channel::GetActiveEvents() {
  return revents_;
}

void Channel::EnableReading() {
  events_ |= kReadEvent;
  if(is_polled_) {
    epoll_ -> ModifyEpollEvent(this, events_);
  } else {
    epoll_ -> AddToEpoll(this, events_);
  }
}

void Channel::EnableETReading() {
  events_ |= KETReadEvent;
  set_fd_noblocked();
  if(is_polled_) {
    epoll_ -> ModifyEpollEvent(this, events_);
  } else {
    epoll_ -> AddToEpoll(this, events_);
  }
}


void Channel::set_fd_noblocked() {
  int flags = fcntl(fd_, F_GETFL, 0);
  flags |= O_NONBLOCK;
  fcntl(fd_, F_SETFL, flags);
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



} // namespace xtc
