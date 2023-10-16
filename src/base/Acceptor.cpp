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
  LOG4CXX_DEBUG(Logger::GetLogger(), "accept channel " << accept_channel_->GetFd() << " register new connection callback");
  callback_ = cb;
}

void Acceptor::on_accept_new_connection() {
  callback_(socket_);
}





} // namespace xtc

