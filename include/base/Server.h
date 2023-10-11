#ifndef __XTC__SERVER__H__
#define __XTC__SERVER__H__

#include "base/EventLoop.h"
#include "base/Socket.h"


namespace xtc{

class Server{
public:
  Server(EventLoop* loop);
  ~Server();
  void Start();

private:
  /// @brief Channel默认新连接的回调函数
  void on_new_connection(Channel* ch);
  /// @brief  Channel默认已连接客户端的可读事件回调函数
  void on_new_message(Channel* ch);

private:

  constexpr static int32_t KMaxBufSize = 1024;

  EventLoop* loop_; // 事件循环
  Socket* socket_; // 用于监听新连接的socket


};



} // namespace xtc


#endif // __XTC__SERVER__H__