#include "base/Socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include "base/InetAddress.h"
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
    auto& addr_in= address.Getaddr();
    errif(::bind(fd_, (struct sockaddr*)(&addr_in), sizeof(addr_in)), "socket bind filed");
  }

  void Socket::listen() {
    errif(::listen(fd_, SOMAXCONN), "socket listen failed");
  }

  int32_t Socket::accept(InetAddress const& address){
    auto& addr_in= address.Getaddr();
    socklen_t client_len;
    int32_t client_fd = ::accept(fd_, (struct sockaddr*)(&addr_in), &client_len);
    errif(client_fd , "scoket accept failed");
    return client_fd;
  }


} // namespace xtc

