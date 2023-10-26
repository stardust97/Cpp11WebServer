#include "base/Server.h"

#include <cstring>

#include "base/InetAddress.h"
#include "base/Socket.h"
#include "base/Epoll.h"
#include "base/Acceptor.h"
#include "util/Logger.h"

namespace xtc {

Server::Server(EventLoop* loop) :main_reactor_ (loop), acceptor_ (nullptr), pool_(nullptr){
  acceptor_ = new Acceptor(loop);
  auto new_connect_callback = std::bind(&Server::on_new_connection, this, std::placeholders::_1);
  acceptor_ -> SetAcceptCallback(new_connect_callback);

  int32_t thread_num = std::thread::hardware_concurrency();
  pool_ = new ThreadPool(thread_num);
  // sub_reactor_ = std::vector<EventLoop*> (thread_num);  //BUG 这样初始化sub_reactor_会导致EventLoop为空指针
  for(int32_t i = 0; i < thread_num;++i) { //subReactor个数和线程池个数一致
    sub_reactor_.push_back(new EventLoop());
    pool_ -> AddTask(std::bind(&EventLoop::Loop, sub_reactor_[i]) );
  }
}

void Server::Start() {
  
}

Server::~Server() {
  delete acceptor_;
  delete pool_; // MARK 先delete pool_ 否则会导致执行空指针？
  for (size_t i = 0; i < sub_reactor_.size(); ++i) {
    delete sub_reactor_[i];
  }

}

void Server::on_new_connection(Socket* socket) {
  int32_t client_fd = socket->GetFd();
  int32_t random = client_fd % sub_reactor_.size(); // 当前使用简单的哈希算法实现负载分配
  Connection* conn = new Connection(sub_reactor_[random], socket);
  conn -> SetNewMsgCallback(on_connect_callback_);
  // TODO 关闭连接的回调函数可以用Channel中的CloseCallback
  conn ->SetDisconnectCallback(std::bind(&Server::on_close_connection, this, std::placeholders::_1));
  connections_.insert(std::make_pair(socket->GetFd(), conn));
}

void Server::on_close_connection(Channel* ch) {
  LOG4CXX_INFO(Logger::GetLogger(),"connection closed, fd: " << ch->GetFd());
  Connection* conn = connections_[ch->GetFd()];
  connections_.erase(ch->GetFd());
  delete conn;
}



} // namespace xtc
