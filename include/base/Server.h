#ifndef __XTC__SERVER__H__
#define __XTC__SERVER__H__

#include <map>

#include "base/EventLoop.h"
#include "base/Socket.h"
#include "base/Acceptor.h"
#include "base/Connection.h"
#include "base/ThreadPool.h"

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
  EventLoop* main_reactor_; // 负责新链接的接收并将其分发给subReactor
  Acceptor* acceptor_; // 用于分发新连接的连接器
  ThreadPool* pool_; // 线程池 subReactor 在线程池中处理客户端事件
  ConnectionList connections_; // 保存当前建立的客户端连接，key为fd，valu为连接
  std::vector<EventLoop*> sub_reactor_; // 负责客户端的请求



};



} // namespace xtc


#endif // __XTC__SERVER__H__