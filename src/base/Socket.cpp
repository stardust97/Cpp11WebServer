#include "base/Socket.h"

#include <memory>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "util/Logger.h"


namespace xtc{
  Socket::Socket() :fd_(-1) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fd_ = sockfd;
  }

  Socket::Socket(int32_t fd) :fd_(fd) {

  }


  Socket::~Socket() {
    if (fd_!=-1) {// BUG 析构退出时关闭了文件描述符，导致连接中断
      LOG4CXX_INFO(Logger::GetLogger(),"socket shutdown, fd: " << fd_);
      ::close(fd_);
    }
  }

  void Socket::Bind(InetAddress& address) {
    auto& addr_in= address.GetAddr();
    int32_t bind_res = ::bind(fd_, (struct sockaddr*)(&addr_in), sizeof(addr_in));
    if(bind_res <0 ) {
      LOG4CXX_ERROR(Logger::GetLogger(),"bind failed");
    }
  }

  void Socket::Listen() {
    int32_t listen_res = ::listen(fd_, SOMAXCONN);
    if(listen_res == -1) {
      LOG4CXX_ERROR(Logger::GetLogger(),"listen failed");
    }
  }

  int32_t Socket::Accept(InetAddress* address) {
    auto& addr_in= address -> GetAddr(); //FIXED GetAddr现在返回的是引用，而不是const引用
    // FIXME const变量转换成指针，其结果是未定义的！！
    socklen_t client_len = sizeof(addr_in); // BUG 已解决accept 返回-1：client_len需要初始化
    int32_t client_fd = ::accept(fd_, (struct sockaddr*)(&addr_in), &client_len);
    if(client_fd < 0) {
      LOG4CXX_ERROR(Logger::GetLogger(),"accept failed");
    }
    LOG4CXX_INFO(Logger::GetLogger(), " client fd: " << client_fd << " ip: " \
        << inet_ntoa(addr_in.sin_addr) << " port: " << ntohs(addr_in.sin_port));
    return client_fd;
  }

  void Socket::Connect(InetAddress* address) {
    auto so_addr = address -> GetAddr();
    int32_t conn_res = ::connect(fd_, reinterpret_cast<sockaddr*>(&so_addr), sizeof(so_addr));
    if(conn_res < 0) {
      LOG4CXX_ERROR(Logger::GetLogger(), "connect failed");
    }
  }


} // namespace xtc

