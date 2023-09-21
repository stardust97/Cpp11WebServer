
#include <memory>

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



private:
  int32_t fd_;
  std::shared_ptr<InetAddress> address_;
};


  
} // namespace xtc
