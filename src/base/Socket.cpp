#include "base/Socket.h"

#include <memory>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "util/Logger.h"


namespace xtc{
  Socket::Socket() :fd_(-1), address_(nullptr) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fd_ = sockfd;
  }

  Socket::Socket(int32_t fd) :fd_(fd) {

  }
  
  Socket::Socket(InetAddress const& address) {
    int32_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fd_ = sockfd;
    address_ = std::make_shared<InetAddress>(address);
  }

  Socket::~Socket() {
    if (fd_!=-1) {
      ::close(fd_);
    }
  }

  void Socket::bind(InetAddress const& address) {
    address_ = std::make_shared<InetAddress>(address);
    auto& addr_in= address_->GetAddr();
    int32_t bind_res = ::bind(fd_, (struct sockaddr*)(&addr_in), sizeof(addr_in));
    if(bind_res <0 ) {
      LOG4CXX_ERROR(Logger::GetLogger(),"bind failed");
    }
  }

  void Socket::listen() {
    int32_t listen_res = ::listen(fd_, SOMAXCONN);
    if(listen_res == -1) {
      LOG4CXX_ERROR(Logger::GetLogger(),"listen failed");
    }
  }

  int32_t Socket::Accept(InetAddress& address) {
    auto& addr_in= address.GetAddr(); //FIXED GetAddr现在返回的是引用，而不是const引用
    // FIXME const变量转换成指针，其结果是未定义的！！
    socklen_t client_len = sizeof(addr_in); // BUG 已解决accept 返回-1：client_len需要初始化
    int32_t client_fd = ::accept(fd_, (struct sockaddr*)(&addr_in), &client_len);
    if(client_fd < 0) {
      LOG4CXX_ERROR(Logger::GetLogger(),"accept failed");
    }

    return client_fd;
  }

  void  Socket::Connect(InetAddress* address) {
    auto so_addr =  address->GetAddr();
    int32_t conn_res = ::connect(fd_, reinterpret_cast<sockaddr*>(&so_addr), sizeof(so_addr));
    
  }

} // namespace xtc

