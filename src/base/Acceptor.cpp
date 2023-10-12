#include "base/Acceptor.h"

namespace xtc {
  

Acceptor::Acceptor(EventLoop *loop):loop_(loop), socket_(nullptr),
    channel_(nullptr), address_(nullptr) {
  socket_ = new Socket();
  address_ = new InetAddress("127.0.0.1", 8888);
  socket_ -> bind(*address_);
  socket_ -> listen();
  Channel* service_channel = new Channel(loop_, socket_->GetFd()); // BUG 内存泄漏
  service_channel->EnableReading(); //阻塞 LT模式
  service_channel -> SetReadCallback(std::bind(&Acceptor::on_accept_new_connection, this));
  printf("ready to accept \n");
}

Acceptor::~Acceptor() {

}

void Acceptor::SetAcceptCallback(NewConnectionCallback const& cb) {
  callback_ = cb;
}

void Acceptor::on_accept_new_connection() {
  callback_(socket_);
}





} // namespace xtc

