#include "base/Socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#include "base/InetAddress.h"






namespace xtc{
  Socket::Socket() :fd_(-1) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    fd_ = sockfd;
  }

  void Socket::bind(InetAddress const& address) {
    struct sockaddr_in addr;
    addr.sin_family = address.family();
    addr.sin_port = htons(address.addr_.sin_port);
    addr.sin_addr.s_addr = htonl(address.getAddress());
    int ret = ::bind(fd_, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
      throw std::runtime_error("bind failed");
    }
  }



} // namespace xtc

