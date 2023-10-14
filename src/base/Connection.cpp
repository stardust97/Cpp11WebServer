#include "base/Connection.h"

#include <cstring>
#include "base/Buffer.h"

namespace xtc{
Connection::Connection(EventLoop* loop, int32_t fd):loop_(loop), ch_(nullptr),read_buf_(nullptr) {
  read_buf_ = new Buffer();
  ch_ = new xtc::Channel(loop_, fd);
  ch_ -> EnableETReading();
  ch_ -> SetReadCallback(std::bind(&Connection::on_new_message, this));
}

Connection::~Connection() {
  delete ch_;
}

void Connection::SetDisconnectCallback(DisConnectCallback cb) {
  printf("set close connection callback\n");
  dis_conn_callback_ = cb;
}


void Connection::on_new_message() {
  printf("client has new msg\n");
  char buf[KMaxBufSize];
  while (true) { // ET模式需要一次性读出所有数据
    bzero(buf, KMaxBufSize);
    int32_t client_fd =  ch_ -> GetFd();
    int32_t read_bytes = read(client_fd, buf, KMaxBufSize);
    if(read_bytes > 0) {
      read_buf_ ->Append(buf, read_bytes);
    } else if(read_bytes == 0) {  //EOF，客户端主动断开连接
      printf("EOF, client fd %d disconnected\n", client_fd);
      // 当close是真的释放了文件描述符资源，而不是减少文件描述的引用计数的话，就会自动从epoll 监听文件描述符集合中删除。
      // close(client_fd); 
      dis_conn_callback_(ch_); //从Server中移除此Connection，释放此Connection资源
      break;
    } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
      //非阻塞IO，这个条件表示本次数据全部读取完毕
      printf("client %d reading once, errno: %d\n",client_fd, errno);
      printf("client fd: %d, recv msg: %s\n", client_fd, read_buf_->GetStr());
      read_buf_ -> Clear();
      break;
    } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
      printf("need next read\n");
      break;
    } 
  }
}


} // namespace xtc

  
  
  
