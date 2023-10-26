#ifndef __XTC__SOCKET__H
#define __XTC__SOCKET__H

#include <memory>

#include "base/InetAddress.h"
#include "base/Channel.h"

namespace xtc{

// support TCP only
class Socket {
public:
  Socket();
  Socket(InetAddress const& address);
  Socket(int32_t fd);
  ~Socket();

  void bind(InetAddress const& address);
  void listen();
  int32_t Accept(InetAddress& address);
  int32_t const& GetFd() {return fd_;} ;
  void Connect(InetAddress* address);


private:
  int32_t fd_;
  std::shared_ptr<InetAddress> address_;
};


  
} // namespace xtc

#endif