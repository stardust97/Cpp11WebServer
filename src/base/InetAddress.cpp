#include "base/InetAddress.h"

#include <string>

#include <cstring>

#include <arpa/inet.h>

namespace xtc{
  
  InetAddress::InetAddress(std::string const& address, uint16_t port) 
      :port_(port), address_(address) {
    struct sockaddr_in  serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPV4
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr(address.c_str());
  }









} // namespace xtc
