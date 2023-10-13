#ifndef __XTC__SERVER__H__
#define __XTC__SERVER__H__

#include <map>

#include "base/EventLoop.h"
#include "base/Socket.h"
#include "base/Acceptor.h"
#include "base/Connection.h"

namespace xtc{

class Server{
  using ConnectionList = std::map<int, Connection*>;
public:
  Server(EventLoop* loop);
  ~Server();
  void Start();

private:
  /// @brief 新连接的回调函数
  void on_new_connection(Socket* socket);

  void on_close_connection(Channel* ch);


private:

  EventLoop* loop_; // 事件循环
  Acceptor* acceptor_; // 用于分发新连接的连接器
  ConnectionList connections_; // 保存当前建立的客户端连接，key为fd，valu为连接
};



} // namespace xtc


#endif // __XTC__SERVER__H__