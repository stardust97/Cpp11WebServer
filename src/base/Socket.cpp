#include "base/Socket.h"

#include <memory>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "util/error_process.h"


namespace xtc{
  Socket::Socket() :fd_(-1), address_(nullptr) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fd_ = sockfd;
  }

  Socket::Socket(InetAddress const& address) {
    int32_t sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fd_ = sockfd;
    address_ = std::make_shared<InetAddress>(address);
  }

  void Socket::bind(InetAddress const& address) {
    address_ = std::make_shared<InetAddress>(address);
    auto& addr_in= address_->GetAddr();
    errif(::bind(fd_, (struct sockaddr*)(&addr_in), sizeof(addr_in)), "socket bind filed");
  }

  void Socket::listen() {
    errif(::listen(fd_, SOMAXCONN), "socket listen failed");
  }

  int32_t Socket::accept(InetAddress& address) {
    auto& addr_in= address.GetAddr(); //FIXED GetAddr现在返回的是引用，而不是const引用
    // FIXME const变量转换成指针，其结果是未定义的！！
    socklen_t client_len = sizeof(addr_in); // BUG 解决accept 返回-1
    int32_t client_fd = ::accept(fd_, (struct sockaddr*)(&addr_in), &client_len);
    errif(client_fd == -1 , "scoket accept failed");
    return client_fd;
  }


} // namespace xtc

