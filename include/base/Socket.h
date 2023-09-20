


namespace xtc{

// support TCP only
class Socket {
public:
  Socket() ;
  ~Socket() = default;

  void bind(InetAddress const& address);
  void listen();
  void accept();
private:
  int32_t fd_;
};


  
} // namespace xtc
