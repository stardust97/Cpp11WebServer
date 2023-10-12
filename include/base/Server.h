#ifndef __XTC__SERVER__H__
#define __XTC__SERVER__H__

#include "base/EventLoop.h"
#include "base/Socket.h"
// #include "base/Acceptor.h
#include "base/Acceptor.h"


namespace xtc{

class Server{
public:
  Server(EventLoop* loop);
  ~Server();
  void Start();

private:
  /// @brief 新连接的回调函数
  void on_new_connection(Socket* socket);
  /// @brief  已连接客户端的可读事件回调函数
  void on_new_message(Channel* ch);

private:

  constexpr static int32_t KMaxBufSize = 1024;

  EventLoop* loop_; // 事件循环
  Acceptor* acceptor_; // 用于分发新连接的连接器

};



} // namespace xtc


#endif // __XTC__SERVER__H__