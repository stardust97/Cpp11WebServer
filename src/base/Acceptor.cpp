#include "base/Acceptor.h"

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
  printf("ready to accept \n");
}

Acceptor::~Acceptor() {
  delete socket_;
  delete accept_channel_;
}

void Acceptor::SetAcceptCallback(NewConnectionCallback const& cb) {
  callback_ = cb;
}

void Acceptor::on_accept_new_connection() {
  callback_(socket_);
}





} // namespace xtc

