#ifndef __XTC__CONNECTION__H__
#define __XTC__CONNECTION__H__

#include <functional>

#include "base/Channel.h"
#include "base/EventLoop.h"
namespace xtc {
class Connection {
  using DisConnectCallback = std::function<void(Channel*)>;

public:
  Connection(EventLoop* loop, int32_t fd);
  ~Connection();
  void SetDisconnectCallback(DisConnectCallback cb);

private:
  void on_new_message();

  constexpr static int32_t KMaxBufSize = 1024;
  EventLoop* loop_;
  Channel* ch_; // 客户端连接的Channel
  DisConnectCallback dis_conn_callback_;
};




} // namespace xtc



#endif // __XTC__CONNECTION__H__