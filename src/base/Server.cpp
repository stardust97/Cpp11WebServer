#include "base/Server.h"

#include <cstring>

#include "base/InetAddress.h"
#include "base/Socket.h"
#include "base/Epoll.h"

namespace xtc {

Server::Server(EventLoop* loop) :loop_ (loop), socket_ (new Socket()) {

}

void Server::Start() {
  InetAddress addr("127.0.0.1", 8888);
  socket_ -> bind(addr);
  socket_ -> listen();
  Channel* service_channel = new Channel(loop_, socket_->GetFd()); // BUG 内存泄漏
  service_channel->EnableReading(); //阻塞 LT模式
  service_channel -> SetReadCallback(std::bind(&Server::on_new_connection, this, std::placeholders::_1));
  printf("server start\n");
}

Server::~Server() {
  //TODO:

}

// 目前ch没有用处，只是为了统一新消息和新链接的接口
void Server::on_new_connection(Channel* ch) {
  InetAddress addr;
  int32_t client_fd = socket_->Accept(addr); 
  auto& client_addr = addr.GetAddr();
  printf("new client fd %d! IP: %s Port: %d\n", client_fd, 
      inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
  Channel* client_channel = new xtc::Channel(loop_, client_fd); // BUG 当前存在内存泄漏
  client_channel -> EnableETReading();
  client_channel -> SetReadCallback(std::bind(&Server::on_new_message, this, std::placeholders::_1));
}

void Server::on_new_message(Channel* ch) {
  printf("client has new msg\n");
  char buf[KMaxBufSize];
  while (true) { // ET模式需要一次性读出所有数据
    bzero(buf, KMaxBufSize);
    int32_t client_fd =  ch -> GetFd();
    int32_t read_bytes = read(client_fd, buf, KMaxBufSize);
    if(read_bytes > 0) {
      printf("client fd: %d, recv msg: %s\n", client_fd, buf);
    } else if(read_bytes == 0) {  //EOF，客户端断开连接
      printf("EOF, client fd %d disconnected\n", client_fd);
      // 当close是真的释放了文件描述符资源，而不是减少文件描述的引用计数的话，就会自动从epoll 监听文件描述符集合中删除。
      close(client_fd); 
      break;
    } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
      //非阻塞IO，这个条件表示本次数据全部读取完毕
      printf("client %d read finished!\n", client_fd);
      break;
    } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
      printf("need next read\n");
      break;
    } 
  }
}

} // namespace xtc
