#include <string>



namespace xtc{

// IPV4 only
class InetAddress {
public:
  InetAddress() = default;
  InetAddress(std::string const& address, uint16_t port);
  ~InetAddress() = default;

  uint16_t const& port() const { return port_;}
  std::string const& address() const { return address_;}
  sa_family_t family() const { return addr_.sin_family; }

private:
  union {
    struct sockaddr_in addr_;
    struct sockaddr_in6 addr6_;
  };
};










} // namespace xtc