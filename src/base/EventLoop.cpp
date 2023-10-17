#include "base/EventLoop.h"

#include <vector>

#include "base/Channel.h"
#include "util/Logger.h"

namespace xtc{

EventLoop::EventLoop(): ep_(nullptr), pool_(nullptr), quit_(false) {
  ep_ = new Epoll();
  pool_ = new ThreadPool();
}

EventLoop::~EventLoop() {
  delete ep_;
  delete pool_;
}

void EventLoop::Loop() {
  LOG4CXX_INFO(Logger::GetLogger(), "start to loop");
  while (!quit_) {
    std::vector<Channel*> active_events;
    ep_ -> Wait(active_events);
    for (Channel* ch : active_events) {
      LOG4CXX_DEBUG(Logger::GetLogger(), "fd " << ch->GetFd() << "occur events: " << ch->GetEvents());
      std::function<void(void)> t = std::bind(&Channel::HandleEvents, ch);
      LOG4CXX_DEBUG(Logger::GetLogger(), "start to add task");

      pool_ -> AddTask(t);
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
