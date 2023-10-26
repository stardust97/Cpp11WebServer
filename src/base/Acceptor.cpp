#include "base/Acceptor.h"
#include "util/Logger.h"

namespace xtc {

Acceptor::Acceptor(EventLoop *loop):loop_(loop), socket_(nullptr),
    accept_channel_(nullptr) {
  socket_ = new Socket();
  InetAddress address ("127.0.0.1", 8888);
  socket_ -> bind(address);
  socket_ -> listen();
  accept_channel_ = new Channel(loop_, socket_->GetFd());
  accept_channel_ -> EnableReading(); //阻塞 LT模式
  accept_channel_ -> SetReadCallback(std::bind(&Acceptor::on_accept_new_connection, this));
  LOG4CXX_INFO(Logger::GetLogger(), "start to accept");
}

Acceptor::~Acceptor() {
  delete socket_;
  delete accept_channel_;
}

void Acceptor::SetAcceptCallback(NewConnectionCallback const& cb) {
  LOG4CXX_DEBUG(Logger::GetLogger(), "accept channel " << accept_channel_->GetFd() 
      << " register new connection callback");
  callback_ = cb;
}

void Acceptor::on_accept_new_connection() {
  InetAddress cli_addr;
  int32_t client_fd = socket_->Accept(cli_addr);
  auto& client_addr = cli_addr.GetAddr();
  LOG4CXX_INFO(Logger::GetLogger(), " client fd: " << client_fd << " ip: " \
      << inet_ntoa(client_addr.sin_addr) << " port: " << ntohs(client_addr.sin_port));
  // Connection* conn = new Connection(sub_reactor_[random], client_fd);
  Socket *client_socket= new Socket(client_fd);
  callback_(client_socket);
}





} // namespace xtc

