#ifndef __XTC__ACCEPTOR__H__
#define __XTC__ACCEPTOR__H__


#include <functional>

#include "base/EventLoop.h"
#include "base/Socket.h"
#include "base/Channel.h"
#include "base/InetAddress.h"

namespace xtc{

class Acceptor {
  using NewConnectionCallback = std::function<void(Socket*)>;
public:
  Acceptor(EventLoop* loop);
  ~Acceptor();

  void SetAcceptCallback(NewConnectionCallback const& cb);
private:
  void on_accept_new_connection();
  EventLoop* loop_;
  Socket* socket_;
  Channel* channel_;
  InetAddress* address_;
  NewConnectionCallback callback_; // 发生新连接时的函数
};



} // namespace xtc



#endif // __XTC__ACCEPTOR__H__