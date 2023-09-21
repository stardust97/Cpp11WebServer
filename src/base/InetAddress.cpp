#include "base/InetAddress.h"

#include <string>
#include <cstring>

#include <arpa/inet.h>

namespace xtc{
  
InetAddress::InetAddress(std::string const& address, uint16_t port)  {
  bzero(&addr_,sizeof(addr_));
  addr_.sin_family = AF_INET; // IPV4
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = inet_addr(address.c_str());
}


} // namespace xtc
