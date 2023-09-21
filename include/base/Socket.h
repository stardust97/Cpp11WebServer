#ifndef __XTC__SOCKET__H
#define __XTC__SOCKET__H


#include <memory>

#include "base/InetAddress.h"


namespace xtc{

// support TCP only
class Socket {
public:
  Socket();
  Socket(InetAddress const& address);
  ~Socket() = default;

  void bind(InetAddress const& address);
  void listen();
  int32_t accept(InetAddress const& address);
  int32_t const& GetFd() {return fd_;} ;


private:
  int32_t fd_;
  std::shared_ptr<InetAddress> address_;
};


  
} // namespace xtc

#endif