#include "base/Channel.h"

#include <memory>
#include "Channel.h"



namespace xtc{  

Channel::Channel(Epoll* ep, int32_t fd) :
    epoll_(std::shared_ptr<Epoll>(ep)), fd_(fd) {

}

void Channel::SetRevents(uint32_t events){
  revents_ = events;
}

void Channel::EnableReading() {
  events_ |= EPOLLIN ;
  if(is_polled_) {
    epoll_ -> ModifyEpollEvent(fd_, events_);
  } else {
    epoll_ -> AddToEpoll(fd_, events_);
  }
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
