#include "base/EventLoop.h"

#include <vector>

#include "base/Channel.h"
#include "util/Logger.h"

namespace xtc{

EventLoop::EventLoop(): ep_(nullptr), quit_(false) {
  ep_ = new Epoll();
}

EventLoop::~EventLoop() {
  delete ep_;
}

void EventLoop::Loop() {
  LOG4CXX_INFO(Logger::GetLogger(), "start to loop");
  while (!quit_) {
    std::vector<Channel*> active_events;
    ep_ -> Wait(active_events);
    for (Channel* ch : active_events) {
      ch -> HandleEvents();
    }
  }
}

void EventLoop::UpdateChannel(Channel* ch) {
  ep_ -> UpdateChannel(ch);
}


void EventLoop::DisableChannel(Channel* channel) {
  ep_ -> RemoveFromEpoll(channel); //
}


} // namespace xtc
