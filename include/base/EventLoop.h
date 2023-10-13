#ifndef __XTC__EVENTLOOP__H__
#define __XTC__EVENTLOOP__H__

#include "base/Epoll.h"

namespace xtc{
class EventLoop {
public:
  EventLoop();
  ~EventLoop();
  void Loop();
  void UpdateChannel(Channel* channel);
  void DisableChannel(Channel* channel);
private:
  Epoll *ep_;
  bool quit_;

};


  
} // namespace xtc


#endif // __XTC__EVENTLOOP__H__