#include "base/Connection.h"

#include <cstring>

#include "base/Buffer.h"
#include "util/Logger.h"

namespace xtc{

Connection::Connection(EventLoop* loop, Socket*  socket):loop_(loop), socket_(nullptr),
    read_buf_(nullptr), write_buf_(nullptr), state_(ConnState::CONNECTED){
  read_buf_ = std::make_unique<Buffer>();
  write_buf_ = std::make_unique<Buffer>();
  socket_  = std::unique_ptr<Socket>(socket);
  ch_ = std::make_unique<Channel>(loop_, socket->GetFd());
  if(loop_) {
    ch_ -> EnableETReading();
  }
}


Connection::~Connection() {
  LOG4CXX_INFO(Logger::GetLogger(), "connection closed");
}

void Connection::SetNewMsgCallback(NewMsgCallback cb) {
  new_msg_callback_ = cb;
  ch_ -> SetReadCallback([this] () {new_msg_callback_(this);}); //MARK 
}

void Connection::SetDisconnectCallback(DisConnectCallback cb) {
  LOG4CXX_INFO(Logger::GetLogger(), "register disconnect callback");
  dis_conn_callback_ = cb;
}

void Connection::Read() {
  if (ch_ ->GetIsBlocked()) {
    blocked_read();
  } else {
    no_blocked_read();
  }
  LOG4CXX_DEBUG(Logger::GetLogger(), "client fd: " << ch_->GetFd() << " recv msg: " << read_buf_->GetStr() );
}

void Connection::Close() {
  LOG4CXX_INFO(Logger::GetLogger(), "close connection, fd: " << ch_ ->GetFd());
  dis_conn_callback_(ch_.get());
}

void Connection::blocked_read() {
  read_buf_ -> Clear();
  char buf[KMaxBufSize];
  bzero(buf, KMaxBufSize);
  int32_t read_bytes = read(ch_ ->GetFd(), buf, KMaxBufSize);
  if(read_bytes > 0) {
    LOG4CXX_DEBUG(Logger::GetLogger(), ", client fd: " <<ch_ ->GetFd() << " read bytes: " << read_bytes); 
    read_buf_ ->Append(buf, read_bytes);
  } else if (read_bytes == -1) {
    state_ = ConnState::CLOSED;
    LOG4CXX_FATAL(Logger::GetLogger(), ", client fd: " <<ch_ ->GetFd() << " read failed: " << errno);
  }
}

void Connection::no_blocked_read() {
  read_buf_ -> Clear();
  char buf[KMaxBufSize];
  while (true) {// ET非阻塞模式需要一次性读出所有数据
    int32_t client_fd = ch_ -> GetFd();
    int32_t read_bytes = read(client_fd, buf, KMaxBufSize);
    if(read_bytes > 0) {
      read_buf_ ->Append(buf, read_bytes);
    } else if(read_bytes == 0) {  //EOF，客户端主动断开连接
      LOG4CXX_INFO(Logger::GetLogger(), "EOF, client fd " << client_fd << " disconnected");
      state_ = ConnState::CLOSED;
      break;
    } else if(read_bytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
      //非阻塞IO，这个条件表示本次数据全部读取完毕
      LOG4CXX_TRACE(Logger::GetLogger(), ", client fd: " << client_fd << " recv msg: " << read_buf_->GetStr() );
      break;
    } else if(read_bytes == -1 && errno == EINTR){  //客户端正常中断、继续读取
      LOG4CXX_DEBUG(Logger::GetLogger(), ", client fd: " << client_fd << " EINTR" ); //MARK 未显示？
      continue;
    } else {
      LOG4CXX_DEBUG(Logger::GetLogger(), ", client fd: " << client_fd << " unkonwn event : " << errno );
    }
  }
  LOG4CXX_DEBUG(Logger::GetLogger(), "receive message: client fd: " << ch_ ->GetFd() 
      << " message: " << read_buf_->GetStr());
}

void Connection::Write() {
  if (ch_ ->GetIsBlocked()) {
    blocked_write();
  } else {
    no_blocked_write();
  }
  LOG4CXX_DEBUG(Logger::GetLogger(),"send message: " << write_buf_ ->GetStr() );
}


void Connection::SetWriteBuf(const char* buf) {
  write_buf_ -> Clear();
  write_buf_ -> Append(buf, strlen(buf));
}
  
void Connection::SetWriteBuf(std::string const& str) {
  write_buf_ -> Clear();
  write_buf_ -> Append(str.c_str(), str.size());
}

void Connection::blocked_write() {
  int32_t write_bytes = ::write(ch_->GetFd(), write_buf_->GetStr(), write_buf_->GetSize());
  if(write_bytes == -1) {
    LOG4CXX_ERROR(Logger::GetLogger(), "Failed to write: " << errno);
    return;
  }
  LOG4CXX_INFO( Logger::GetLogger(), "Send message: " << write_buf_ ->GetStr() );
}

void Connection::no_blocked_write() {
  int32_t total_size = write_buf_ ->GetSize(), send_size = 0;
  while (send_size < total_size ) {
    int32_t write_bytes = ::write(ch_->GetFd(),
        write_buf_->GetStr()+send_size, write_buf_->GetSize()-send_size);
    if(write_bytes == -1 ) {
      if((errno == EINTR || errno == EAGAIN)) {
        LOG4CXX_DEBUG(Logger::GetLogger(), "write info: " << errno); 
        continue;
      }
      else {
        LOG4CXX_FATAL(Logger::GetLogger(), "write error: " << errno); 
        break;
      }
    }
    send_size += write_bytes;
  }
}



} // namespace xtc

  
  
  
