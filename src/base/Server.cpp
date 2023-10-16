#include "base/Server.h"

#include <cstring>

#include "base/InetAddress.h"
#include "base/Socket.h"
#include "base/Epoll.h"
#include "base/Acceptor.h"
#include "util/Logger.h"


namespace xtc {

Server::Server(EventLoop* loop) :loop_ (loop) {
  acceptor_ = new Acceptor(loop);
  auto new_connect_callback = std::bind(&Server::on_new_connection, this, std::placeholders::_1);
  acceptor_ -> SetAcceptCallback(new_connect_callback);
}

void Server::Start() {
  
}

Server::~Server() {
  delete acceptor_;
}

void Server::on_new_connection(Socket* socket) {
  InetAddress cli_addr;
  int32_t client_fd = socket->Accept(cli_addr);
  auto& client_addr = cli_addr.GetAddr();
  LOG4CXX_INFO(Logger::GetLogger(), " client fd: " << client_fd << " ip: " \
      << inet_ntoa(client_addr.sin_addr) << " port: " << ntohs(client_addr.sin_port));
  Connection* conn = new Connection(loop_, client_fd);
  // TODO 关闭连接的回调函数可以用Channel中的CloseCallback
  conn ->SetDisconnectCallback(std::bind(&Server::on_close_connection, this, std::placeholders::_1));
  connections_.insert(std::make_pair(socket->GetFd(), conn));
}

void Server::on_close_connection(Channel* ch) {
  Connection* conn = connections_[ch->GetFd()];
  connections_.erase(ch->GetFd());
  delete conn;
}



} // namespace xtc
